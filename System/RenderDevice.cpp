#include "RenderDevice.h"
#include "d3dx11effect.h"
#include "FXManager.h"
#include "VertexDesc.h"

HWND RenderDevice::_hWnd = nullptr;

RenderDevice::RenderDevice(void)
	: _pd3dDevice(nullptr),
	_pImmediateContext(nullptr),
	_pSwapChain(nullptr),
	_pBackBufferRenderTargetView(nullptr),
	_depthStencilBuffer(nullptr),
	_depthStencilView(nullptr),
	_depthStencilView_ReadOnly(nullptr),
	_depthStencilResourceView(nullptr),
	mPresentFX(nullptr),
	_4xMsaaQuality(0),
	_Enable4xMsaa(false)
{
}

RenderDevice::~RenderDevice(void)
{

}

HRESULT RenderDevice::InitDevice(HWND hWnd)
{
	if (nullptr == hWnd)
	{
		return E_FAIL;
	}

	_hWnd = hWnd;

	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(_hWnd, &rc);
	_Width = rc.right - rc.left;
	_Height = rc.bottom - rc.top;

	// Create the device and device context.

	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	HR(D3D11CreateDevice(
		0,                 // default adapter
		D3D_DRIVER_TYPE_HARDWARE,
		0,                 // no software device
		createDeviceFlags,
		0, 0,              // default feature level array
		D3D11_SDK_VERSION,
		&_pd3dDevice,
		&featureLevel,
		&_pImmediateContext), L"D3D11CreateDevice Failed.");

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}

	// Check 4X MSAA quality support for our back buffer format.
	// All Direct3D 11 capable devices support 4X MSAA for all render 
	// target formats, so we only need to check quality support.

	HR(_pd3dDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &_4xMsaaQuality), L"CheckMultisampleQualityLevels Failed.");

	assert(_4xMsaaQuality > 0);

	// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = _Width;
	sd.BufferDesc.Height = _Height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Use 4X MSAA? 
	if (_Enable4xMsaa)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = _4xMsaaQuality - 1;
	}
	// No MSAA
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = _hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	// To correctly create the swap chain, we must use the IDXGIFactory that was
	// used to create the device.  If we tried to use a different IDXGIFactory instance
	// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
	// This function is being called with a device from a different IDXGIFactory."

	IDXGIDevice* dxgiDevice = 0;
	HR(_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice), L"Device->QueryInterface Fail");

	IDXGIAdapter* dxgiAdapter = 0;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter), L"Device->GetParent dxgiAdapter Fail");

	IDXGIFactory* dxgiFactory = 0;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory), L"dxgiAdapter->GetParent dxgiFactory Fail");

	HR(dxgiFactory->CreateSwapChain(_pd3dDevice, &sd, &_pSwapChain), L"CreateSwapChain Fail");

	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();

	// The remaining steps that need to be carried out for d3d creation
	// also need to be executed every time the window is resized.  So
	// just call the OnResize method here to avoid code duplication.

	HR(ResizeDevice(), L"ResizeDevice Fail");

	//Init FXManager
	HR(FXManager::This().CompileFXFile(), L"Compile FX Fail");
	HR(FXManager::This().LoadFX(), L"Load FX Fail");

	RenderStates::This().InitAll(_pd3dDevice);

	return S_OK;
}

HRESULT RenderDevice::ResizeDevice()
{
	HRESULT hr = S_OK;
	if (nullptr == _pd3dDevice)
	{
		return hr;
	}

	if (nullptr == _pImmediateContext)
	{
		return hr;
	}

	if (nullptr == _pSwapChain)
	{
		return hr;
	}

	// Release the old views, as they hold references to the buffers we
	// will be destroying.  Also release the old depth/stencil buffer.

	if (_pBackBufferRenderTargetView)
		hr = _pBackBufferRenderTargetView->Release();

	if (FAILED(hr))
		return hr;

	if (_depthStencilView)
		hr = _depthStencilView->Release();

	if (_depthStencilView_ReadOnly)
		hr = _depthStencilView_ReadOnly->Release();

	if (_depthStencilResourceView)
		hr = _depthStencilResourceView->Release();

	if (FAILED(hr))
		return hr;

	if (_depthStencilBuffer)
		hr = _depthStencilBuffer->Release();

	if (FAILED(hr))
		return hr;

	RECT		rc;
	GetClientRect(_hWnd, &rc);
	_Width = rc.right - rc.left;
	_Height = rc.bottom - rc.top;

	// Resize the swap chain and recreate the render target view.

	hr = _pSwapChain->ResizeBuffers(1, _Width, _Height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	if (FAILED(hr))
		return hr;

	ID3D11Texture2D* pBackBuffer;
	hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr))
		return hr;

	hr = _pd3dDevice->CreateRenderTargetView(pBackBuffer, 0, &_pBackBufferRenderTargetView);
	if (FAILED(hr))
		return hr;

	pBackBuffer->Release();

	// Create the depth/stencil buffer and view.

	DXGI_FORMAT depthStencilTextureDesc = DXGI_FORMAT_R24G8_TYPELESS;
	DXGI_FORMAT depthStencilRenderTargetDesc = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DXGI_FORMAT depthStencilResourceDesc = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = _Width;
	depthStencilDesc.Height = _Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = depthStencilTextureDesc;

	// Use 4X MSAA? --must match swap chain MSAA values.
	if (_Enable4xMsaa)
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = _4xMsaaQuality - 1;
	}
	// No MSAA
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	hr = _pd3dDevice->CreateTexture2D(&depthStencilDesc, 0, &_depthStencilBuffer);
	if (FAILED(hr))
		return hr;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd =
	{
		depthStencilRenderTargetDesc,
		D3D11_DSV_DIMENSION_TEXTURE2D,
		0
	};

	hr = _pd3dDevice->CreateDepthStencilView(_depthStencilBuffer, &dsvd, &_depthStencilView);
	if (FAILED(hr))
		return hr;

	dsvd.Flags = D3D11_DSV_READ_ONLY_DEPTH | D3D11_DSV_READ_ONLY_STENCIL;
	hr = _pd3dDevice->CreateDepthStencilView(_depthStencilBuffer, &dsvd, &_depthStencilView_ReadOnly);
	if (FAILED(hr))
		return hr;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc =
	{
		depthStencilResourceDesc,
		D3D11_SRV_DIMENSION_TEXTURE2D,
		0,
		1
	};
	hr = _pd3dDevice->CreateShaderResourceView(_depthStencilBuffer, &srvDesc, &_depthStencilResourceView);
	if (FAILED(hr))
		return hr;

	// Bind the render target view and depth/stencil view to the pipeline.
	_pImmediateContext->OMSetRenderTargets(1, &_pBackBufferRenderTargetView, _depthStencilView);


	// Set the viewport transform.
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)_Width;
	vp.Height = (FLOAT)_Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	_pImmediateContext->RSSetViewports(1, &vp);

	return S_OK;
}

void RenderDevice::Begin(RenderTargetDesc& rendertargetDesc)
{
	UINT size = (UINT)rendertargetDesc._RenderTargetList.size();
	for (UINT i = 0; i < size; ++i)
	{
		_pImmediateContext->ClearRenderTargetView(rendertargetDesc._RenderTargetList[i], BackBufferColor);
	}

	switch(rendertargetDesc._DepthType)
	{
	case eRTDT_NO_USE_DEPTH:
		_pImmediateContext->OMSetRenderTargets(size, &rendertargetDesc._RenderTargetList[0], NULL);
		break;
	case eRTDT_USE_READONLY_DEPTH:
		_pImmediateContext->OMSetRenderTargets(size, &rendertargetDesc._RenderTargetList[0], _depthStencilView_ReadOnly);
		break;
	case eRTDT_USE_DEPTH:
		_pImmediateContext->OMSetRenderTargets(size, &rendertargetDesc._RenderTargetList[0], _depthStencilView);
		_pImmediateContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		break;
	default:
		MessageBox(0, L"No setting Depth buffer type", 0, 0);
		break;
	}
}

void RenderDevice::End(RenderTargetDesc& rendertargetDesc)
{
	// Little cleanup
	UINT size = (UINT)rendertargetDesc._RenderTargetList.size();
	ZeroMemory(&rendertargetDesc._RenderTargetList[0], sizeof(ID3D11RenderTargetView*) * size);
	_pImmediateContext->OMSetRenderTargets(size, &rendertargetDesc._RenderTargetList[0], _depthStencilView);
}

ID3D11Device* RenderDevice::GetDevice()
{
	return _pd3dDevice;
}

ID3D11DeviceContext * RenderDevice::GetContext()
{
	return _pImmediateContext;
}

IDXGISwapChain * RenderDevice::GetSwapChain()
{
	return _pSwapChain;
}

ID3D11DepthStencilView * RenderDevice::GetDepthStencilView()
{
	return _depthStencilView;
}

ID3D11DepthStencilView * RenderDevice::GetReadOnlyDepthStencilView()
{
	return _depthStencilView_ReadOnly;
}

ID3D11ShaderResourceView * RenderDevice::GetDepthStencilResourceView()
{
	return _depthStencilResourceView;
}

ID3D11RenderTargetView * RenderDevice::GetBackBufferRenderTarget()
{
	return _pBackBufferRenderTargetView;
}

ID3DX11EffectPass* RenderDevice::GetPassByIndex(const UINT index)
{
	return mPresentFX->_FxTech->GetPassByIndex(index);
}

ID3DX11EffectMatrixVariable* RenderDevice::GetVariableByName(const char* varname)
{
	return mPresentFX->_FxEffect->GetVariableByName(varname)->AsMatrix();
}

ID3DX11EffectVariable* RenderDevice::GetRawVariableByName(const char* varname)
{
	return mPresentFX->_FxEffect->GetVariableByName(varname);
}


HRESULT RenderDevice::Render()
{

	return E_NOTIMPL;
}

HRESULT RenderDevice::Begin(std::string fxindex, const int techIndex)
{
	mPresentFX = FXManager::This().GetFx(fxindex, techIndex);

	if (!mPresentFX)
	{
		return E_FAIL;
	}

	_pImmediateContext->IASetInputLayout(mPresentFX->_InputLayer);
	_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//RenderDevice::This().GetContext()->RSSetState(RenderStates::This().WireframeRS);

	return S_OK;
}

HRESULT RenderDevice::End(std::string fxIndex)
{
	return S_OK;
}

void RenderDevice::SetRenderState(eRENDER_STATE renderstate)
{
	switch(renderstate)
	{
	case eRS_WIREFRAME:
		RenderDevice::This().GetContext()->RSSetState(RenderStates::This().WireframeRS);
		break;
	case eRS_NO_CULL:
		RenderDevice::This().GetContext()->RSSetState(RenderStates::This().NoCullRS);
		break;
	case eRS_CLOCKWISE:
		RenderDevice::This().GetContext()->RSSetState(RenderStates::This().CullClockwiseRS);
		break;
	case eRS_NO_USE_DEPTHSTENCIL:
		RenderDevice::This().GetContext()->OMSetDepthStencilState(RenderStates::This().NoUseDepthStencilDSS, 0);
		break;
	case eRS_READONLY_DEPTHSTENCIL:
		RenderDevice::This().GetContext()->OMSetDepthStencilState(RenderStates::This().ReadOnlyDepthStencilDSS, 0);
		break;
	default:
	{
		float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		RenderDevice::This().GetContext()->OMSetBlendState(0, blendFactor, 0xffffffff);
		RenderDevice::This().GetContext()->RSSetState(0);
		RenderDevice::This().GetContext()->OMSetDepthStencilState(0, 0);
	}
		break;
	}
}

void RenderDevice::Release()
{
	_pd3dDevice->Release();
	_pImmediateContext->Release();
	_pSwapChain->Release();
	_pBackBufferRenderTargetView->Release();
	_depthStencilBuffer->Release();
	_depthStencilView->Release();
	//_RSWireFrame->Release();

	FXManager::This().Release();
	RenderStates::This().Release();
}
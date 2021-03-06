#include "stdafx.h"
#include "RenderTargetManager.h"
#include "TextureManager.h"
#include "CameraManager.h"
#include "../System/RenderDevice.h"
#include "../System/VertexDesc.h"

const std::vector<Vertex_RenderTarget> RenderTargetManager::_ScreenQuadVertices =
{
	{ acm::float3(-1,-1,0), acm::float2(0, 1) },
	{ acm::float3(-1,+1,0), acm::float2(0, 0) },
	{ acm::float3(+1,+1,0), acm::float2(1, 0) },
	{ acm::float3(+1,-1,0), acm::float2(1, 1) },
};

const std::vector<UINT> RenderTargetManager::_ScreenQuadIndices =
{
	0, 1, 2,
	0, 2 ,3
};

void RenderTargetManager::Init()
{
	//����Ÿ�� ����
	_RenderTargetList.resize(eRT_COUNT);

	const UINT width = RenderDevice::This().GetWidth();
	const UINT height = RenderDevice::This().GetHeight();

	Texture* pBackBuffer = new Texture();
	pBackBuffer->m_pRenderTargetView = RenderDevice::This().GetBackBufferRenderTarget();
	pBackBuffer->m_TextureSize.x = width;
	pBackBuffer->m_TextureSize.y = height;
	pBackBuffer->m_pRenderTargetView->AddRef();

	_RenderTargetList[eRT_BACKBUFFER] = pBackBuffer;
	_RenderTargetList[eRT_COLOR] = TextureManager::This().CreateRenderTargetTexture(width, height, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
	_RenderTargetList[eRT_NORMAL] = TextureManager::This().CreateRenderTargetTexture(width, height, DXGI_FORMAT_R11G11B10_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
	_RenderTargetList[eRT_SPECPOWER] = TextureManager::This().CreateRenderTargetTexture(width, height, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
	//_RenderTargetList[eRT_BACKBUFFER_TEMP] = TextureManager::This().CreateRenderTargetTexture(width, height, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);
	//_RenderTargetList[eRT_BLUR] = TextureManager::This().CreateRenderTargetTexture(width, height, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);


	//����Ÿ�ٿ� ����
	{
		_ScreenQuadBuffer = new MeshBuffer();
		//���� ���� ����

		D3D11_BUFFER_DESC vBufferDesc;
		vBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vBufferDesc.ByteWidth = sizeof(Vertex_RenderTarget) * (UINT)_ScreenQuadVertices.size();
		vBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vBufferDesc.CPUAccessFlags = 0;
		vBufferDesc.MiscFlags = 0;
		vBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vInitData;
		vInitData.pSysMem = &_ScreenQuadVertices[0];

		if (FAILED(RenderDevice::This().GetDevice()->CreateBuffer(&vBufferDesc, &vInitData, &_ScreenQuadBuffer->vBuffer)))
		{
			_ScreenQuadBuffer->vBuffer->Release();
			_ScreenQuadBuffer->vIBuffer->Release();
			delete _ScreenQuadBuffer;
			return;
		}
	}

	{
		//�ε��� ���� ����
		
		D3D11_BUFFER_DESC vIdxBufferDesc;
		vIdxBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vIdxBufferDesc.ByteWidth = sizeof(UINT) * (UINT)_ScreenQuadIndices.size();
		vIdxBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		vIdxBufferDesc.CPUAccessFlags = 0;
		vIdxBufferDesc.MiscFlags = 0;
		vIdxBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA iIdxInitData;
		iIdxInitData.pSysMem = &_ScreenQuadIndices[0];

		if (FAILED(RenderDevice::This().GetDevice()->CreateBuffer(&vIdxBufferDesc, &iIdxInitData, &_ScreenQuadBuffer->vIBuffer)))
		{
			_ScreenQuadBuffer->vBuffer->Release();
			_ScreenQuadBuffer->vIBuffer->Release();
			delete _ScreenQuadBuffer;
			return;
		}
	}
}

void RenderTargetManager::Release()
{
	for (int i = 0; i < eRT_COUNT; ++i)
	{
		if(nullptr != _RenderTargetList[i]) _RenderTargetList[i]->Release();
	}

	_ScreenQuadBuffer->vBuffer->Release();
	_ScreenQuadBuffer->vIBuffer->Release();
	delete _ScreenQuadBuffer;
}

void RenderTargetManager::DrawScreenQuadRaw()
{	
	UINT stride = sizeof(Vertex_RenderTarget);
	UINT offset = 0;

	acm::float4x4 identity = acm::float4x4(acm::Identity);

	RenderDevice::This().GetContext()->IASetVertexBuffers(0, 1, &_ScreenQuadBuffer->vBuffer, &stride, &offset);
	RenderDevice::This().GetContext()->IASetIndexBuffer(_ScreenQuadBuffer->vIBuffer, DXGI_FORMAT_R32_UINT, 0);

	RenderDevice::This().ApplyFX();
	RenderDevice::This().GetContext()->DrawIndexed(6, 0, 0);
}

void RenderTargetManager::DrawScreenQuad()
{
	RenderDevice::This().BeginFX("UnPackGBufferTech", 0);

	RenderDevice::This().GetRawVariableByName("gDepthTex")->AsShaderResource()->SetResource(RenderDevice::This().GetDepthStencilResourceView());
	RenderDevice::This().GetRawVariableByName("gColorTex")->AsShaderResource()->SetResource(_RenderTargetList[RenderTargetManager::eRT_COLOR]->m_pResourceView);
	RenderDevice::This().GetRawVariableByName("gNormalTex")->AsShaderResource()->SetResource(_RenderTargetList[RenderTargetManager::eRT_NORMAL]->m_pResourceView);
	RenderDevice::This().GetRawVariableByName("gSpecTex")->AsShaderResource()->SetResource(_RenderTargetList[RenderTargetManager::eRT_SPECPOWER]->m_pResourceView);

	const acm::float4x4 pProj = CameraManager::This().GetCurrentCamera()->GetProj();
	acm::float4 PerspectiveValues;
	PerspectiveValues.x = 1.0f / CameraManager::This().GetCurrentCamera()->GetProj()._11;
	PerspectiveValues.y = 1.0f / CameraManager::This().GetCurrentCamera()->GetProj()._22;
	PerspectiveValues.z = CameraManager::This().GetCurrentCamera()->GetProj()._33;
	PerspectiveValues.w = -CameraManager::This().GetCurrentCamera()->GetProj()._33;
	RenderDevice::This().GetRawVariableByName("PerspectiveValues")->AsVector()->SetRawValue(&PerspectiveValues, 0, sizeof(acm::float4));

	acm::float4x4 invView = CameraManager::This().GetCurrentCamera()->GetView().inverse_val();
	RenderDevice::This().GetVariableByName("ViewInv")->SetMatrix(reinterpret_cast<float*>(&invView));

	RenderDevice::This().GetContext()->IASetInputLayout(NULL);
	RenderDevice::This().GetContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	RenderDevice::This().GetContext()->IASetVertexBuffers(0, 0, nullptr, 0, 0);
	RenderDevice::This().ApplyFX();
	RenderDevice::This().GetContext()->Draw(4, 0);

	RenderDevice::This().GetRawVariableByName("gDepthTex")->AsShaderResource()->SetResource(NULL);
	RenderDevice::This().GetRawVariableByName("gColorTex")->AsShaderResource()->SetResource(NULL);
	RenderDevice::This().GetRawVariableByName("gNormalTex")->AsShaderResource()->SetResource(NULL);
	RenderDevice::This().GetRawVariableByName("gSpecTex")->AsShaderResource()->SetResource(NULL);
	RenderDevice::This().ApplyFX();

	RenderDevice::This().EndFX();
}

#include "stdafx.h"
#include "TextureManager.h"
#include <d3d11_1.h>
#include "../CommonUtil/StringUtil.h"
#include "../CommonUtil/CommonDefine.h"
#include "../System/FileSystem.h"
#include "../System/RandomManager.h"
#include "../System/RenderDevice.h"
#include "../System/DDSTextureLoader.h"
#include "../System/ScreenGrab.h"

void TextureManager::SaveTextureFile(Texture * pTex)
{
	ID3D11Resource* pRes = nullptr;

	if(pTex->m_pRenderTargetView)
	{
		pTex->m_pRenderTargetView->GetResource(&pRes);
	}else if(pTex->m_pResourceView)
	{
		pTex->m_pResourceView->GetResource(&pRes);
	}

	if(pRes)
	{
		SaveDDSTextureToFile(RenderDevice::This().GetContext(), pRes, L"test1.dds");
		pRes->Release();
	}
}

Texture * TextureManager::LoadTexture(std::string strFileName)
{
	Texture* pTexture = new Texture();
	pTexture->m_strFileName = strFileName;

	std::string formatname = FileSystem::GetFileFormat(pTexture->m_strFileName);

	HRESULT hr = S_FALSE;

	if(formatname == ".dds")
	{
		HR(CreateDDSTextureFromFile(RenderDevice::This().GetDevice(), Multi2Wide(strFileName.c_str()).c_str(), 0, &(pTexture->m_pResourceView)), L"CreateDDSTextureFromFile Fail");
	}
	else
	{
		HR(CreateWICTextureFromFile(RenderDevice::This().GetDevice(), Multi2Wide(strFileName.c_str()).c_str(), 0, &(pTexture->m_pResourceView)), L"CreateWICTextureFromFile Fail");
	}

	ID3D11Resource* res;
	ID3D11Texture2D* pTextureInterface = 0;
	D3D11_TEXTURE2D_DESC desc;
	pTexture->m_pResourceView->GetResource(&res);
	res->QueryInterface<ID3D11Texture2D>(&pTextureInterface);
	pTextureInterface->GetDesc(&desc);

	pTexture->m_TextureSize.x = desc.Width;
	pTexture->m_TextureSize.y = desc.Height;

	pTextureInterface->Release();
	res->Release();

	return pTexture;
}

Texture* TextureManager::LoadTexture2DArray(std::string textureSetName, std::vector<std::string>& filenames)
{
	std::vector<ID3D11Texture2D*> srcTex(filenames.size());

	UINT index = 0;
	for (auto iter = filenames.begin(); iter != filenames.end(); ++iter)
	{
		std::string formatname = FileSystem::GetFileFormat((*iter));

		HRESULT hr = S_FALSE;
		if (formatname == ".dds")
		{
			// D3D11_USAGE_STAGING 주의
			HR(CreateDDSTextureFromFileEx(RenderDevice::This().GetDevice(), Multi2Wide((*iter).c_str()).c_str(), 0, D3D11_USAGE_STAGING, 0, D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ, 0, false, (ID3D11Resource**)&(srcTex[index]), nullptr), L"CreateDDSTextureFromFileEx Fail");
		}
		else
		{
			HR(CreateWICTextureFromFileEx(RenderDevice::This().GetDevice(), Multi2Wide((*iter).c_str()).c_str(), 0, D3D11_USAGE_STAGING, 0, D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ, 0, false, (ID3D11Resource**)&(srcTex[index]), nullptr), L"CreateWICTextureFromFileEx Fail");
		}

		++index;
	}

	// 텍스처 배열 생성

	D3D11_TEXTURE2D_DESC texElementDesc;
	srcTex[0]->GetDesc(&texElementDesc);

	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc = texElementDesc;

	//중요!! 바뀌는게 많다
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.ArraySize = index;
	texArrayDesc.CPUAccessFlags = 0;

	ID3D11Texture2D* texArray = 0;
	
	HRESULT hr;

	hr = RenderDevice::This().GetDevice()->CreateTexture2D(&texArrayDesc, 0, &texArray);

	// 개별 텍스쳐 원소를 텍스처 배열에 복사

	for (UINT i = 0; i < index; ++i)
	{
		for (UINT mipLevel = 0; mipLevel < texElementDesc.MipLevels; ++mipLevel)
		{
			D3D11_MAPPED_SUBRESOURCE mappedTex2D;

			hr = RenderDevice::This().GetContext()->Map(srcTex[i], mipLevel, D3D11_MAP_READ, 0, &mappedTex2D);

			RenderDevice::This().GetContext()->UpdateSubresource(texArray
				, D3D11CalcSubresource(mipLevel, i, texElementDesc.MipLevels)
				, 0, mappedTex2D.pData, mappedTex2D.RowPitch, mappedTex2D.DepthPitch);

			RenderDevice::This().GetContext()->Unmap(srcTex[i], mipLevel);
		}
	}

	// 텍스처 배열에 대한 자원 뷰

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;

	viewDesc.Format = texArrayDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	viewDesc.Texture2DArray.MostDetailedMip = 0;
	viewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
	viewDesc.Texture2DArray.FirstArraySlice = 0;
	viewDesc.Texture2DArray.ArraySize = index;

	ID3D11ShaderResourceView* texArraySRV = 0;	

	hr = RenderDevice::This().GetDevice()->CreateShaderResourceView(texArray, &viewDesc, &texArraySRV);
	

	//자원 해제

	texArray->Release();
	for (UINT i = 0; i < index; ++i)
	{
		srcTex[i]->Release();
	}

	Texture* pTexture = new Texture();
	pTexture->m_strFileName = textureSetName;
	pTexture->m_pResourceView = texArraySRV;

	return pTexture;
}

Texture * TextureManager::CreateRenderTargetTexture(UINT width, UINT height, DXGI_FORMAT format, UINT BindFlags)
{
	D3D11_TEXTURE2D_DESC texDesc;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	RenderDevice::This().GetSwapChain()->GetDesc(&swapChainDesc);

	// 렌더타겟 샘플링이 같아야한다. 지금은 따로 처리안했으니 msaa끕니다
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = format;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = BindFlags; //D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* renderTargetTex = 0;
	if(FAILED(RenderDevice::This().GetDevice()->CreateTexture2D(&texDesc, 0, &renderTargetTex)))
	{
		//렌더타겟 생성실패
		return nullptr;
	}

	Texture* pTexture = new Texture();

	if(texDesc.BindFlags & D3D11_BIND_RENDER_TARGET)
	{
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc =
		{
			format,
			D3D11_RTV_DIMENSION_TEXTURE2D
		};

		if (FAILED(RenderDevice::This().GetDevice()->CreateRenderTargetView(renderTargetTex, &rtvDesc, &pTexture->m_pRenderTargetView)))
		{
			//렌더타겟 생성실패

			delete pTexture;
			renderTargetTex->Release();

			return nullptr;
		}
	}

	if (texDesc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		if (FAILED(RenderDevice::This().GetDevice()->CreateShaderResourceView(renderTargetTex, &srvDesc, &pTexture->m_pResourceView)))
		{
			//렌더타겟 생성실패

			delete pTexture;
			renderTargetTex->Release();

			return nullptr;
		}
	}

	if (texDesc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		uavDesc.Format = format;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		uavDesc.Texture2D.MipSlice = 0;
		if (FAILED(RenderDevice::This().GetDevice()->CreateUnorderedAccessView(renderTargetTex, &uavDesc, &pTexture->m_pUnorderdView)))
		{
			//렌더타겟 생성실패

			delete pTexture;
			renderTargetTex->Release();

			return nullptr;
		}
	}

	pTexture->m_TextureSize.x = width;
	pTexture->m_TextureSize.y = height;

	return pTexture;
}

Texture * TextureManager::CreateVolumeTexture(UINT width, UINT height, UINT depth, DXGI_FORMAT format, UINT BindFlags, void* pData)
{
	D3D11_TEXTURE3D_DESC texDesc;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	RenderDevice::This().GetSwapChain()->GetDesc(&swapChainDesc);

	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.Depth = depth;
	texDesc.MipLevels = 1;
	texDesc.Format = format;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = BindFlags; //D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture3D* vaolumeTex = 0;

	D3D11_SUBRESOURCE_DATA resData;
	resData.pSysMem = pData;
	resData.SysMemPitch = width * 2 * 4;
	resData.SysMemSlicePitch = width * height * 2 * 4;

	if( FAILED(RenderDevice::This().GetDevice()->CreateTexture3D(&texDesc, &resData, &vaolumeTex)) )
	{
		//볼륨텍스쳐 생성실패
		return nullptr;
	}

	Texture* pTexture = new Texture();

	if (texDesc.BindFlags & D3D11_BIND_RENDER_TARGET)
	{
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		rtvDesc.Format = format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE3D;
		rtvDesc.Texture3D.MipSlice = 0;
		rtvDesc.Texture3D.FirstWSlice = 0;
		rtvDesc.Texture3D.WSize = -1;

		if (FAILED(RenderDevice::This().GetDevice()->CreateRenderTargetView(vaolumeTex, &rtvDesc, &pTexture->m_pRenderTargetView)))
		{
			//렌더타겟 생성실패

			delete pTexture;
			vaolumeTex->Release();

			return nullptr;
		}
	}

	if (texDesc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
		srvDesc.Texture3D.MostDetailedMip = 0;
		srvDesc.Texture3D.MipLevels = 1;
		if (FAILED(RenderDevice::This().GetDevice()->CreateShaderResourceView(vaolumeTex, &srvDesc, &pTexture->m_pResourceView)))
		{
			//렌더타겟 생성실패

			delete pTexture;
			vaolumeTex->Release();

			return nullptr;
		}
	}

	if (texDesc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		uavDesc.Format = format;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
		uavDesc.Texture3D.MipSlice = 0;
		uavDesc.Texture3D.FirstWSlice = 0;
		uavDesc.Texture3D.WSize = 0;
		if (FAILED(RenderDevice::This().GetDevice()->CreateUnorderedAccessView(vaolumeTex, &uavDesc, &pTexture->m_pUnorderdView)))
		{
			//렌더타겟 생성실패

			delete pTexture;
			vaolumeTex->Release();

			return nullptr;
		}
	}

	pTexture->m_TextureSize.x = width;
	pTexture->m_TextureSize.y = height;
	pTexture->m_TextureSize.z = depth;

	return pTexture;
}

Texture* TextureManager::CreateNoiseTexture(UINT width, UINT height)
{
	//float* data = new float[width * height * 3];

	//for (int i = 0; i < width * height * 3; i+=3)
	//{
	//	acm::float3 temp = RandomManager::This().RandomNormal();
	//	data[i] = temp.x;
	//	data[i + 1] = temp.y;
	//	data[i + 2] = temp.z;
	//}
	byte* data = new byte[width * height * 4];

	for (UINT i = 0; i < width * height * 4; i+=4)
	{
		acm::float3 temp = RandomManager::This().RandomNormal();
		temp.x *= 1000;
		temp.y *= 1000;
		temp.z *= 1000;

		data[i] = ((int)temp.x) % 255;
		data[i + 1] = ((int)temp.y) % 255;
		data[i + 2] = ((int)temp.z) % 255;
		data[i + 3] = 0xff;
	}

	Texture* pTexture = new Texture();

	ID3D11Texture2D* pTex;
	D3D11_TEXTURE2D_DESC texDesc;

	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	//texDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE; //D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	HRESULT hr = RenderDevice::This().GetDevice()->CreateTexture2D(&texDesc, 0, &pTex);
	RenderDevice::This().GetContext()->UpdateSubresource(pTex
		, 0
		, 0
		, data
		, width * 4, 0);

	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MostDetailedMip = 0;
	desc.Texture2D.MipLevels = 1;

	RenderDevice::This().GetDevice()->CreateShaderResourceView(pTex, &desc, &pTexture->m_pResourceView);
	pTex->Release();

	SaveTextureFile(pTexture);

	//완성덜됨
	return pTexture;
}

Texture* TextureManager::CreateBufferTexture(TextureUsage texUsage, void* pvBuffer, UINT sizeofBufferType, UINT elementCount)
{
	//실패하면 예외처리
	HRESULT hr;
	Texture* pTexture = new Texture();

	// 세이더의 입력 버퍼 생성
	//step-1
	D3D11_BUFFER_DESC Desc;

	Desc.Usage = D3D11_USAGE_DEFAULT;
	Desc.ByteWidth = sizeofBufferType * elementCount;
	Desc.CPUAccessFlags = 0;
	Desc.StructureByteStride = sizeofBufferType;
	Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	switch(texUsage)
	{
	case eTextureUsageInputBuffer:
		Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		break;
	case eTextureUsageOutputBuffer:
		Desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		break;
	}

	D3D11_SUBRESOURCE_DATA input;
	input.pSysMem = pvBuffer;

	ID3D11Buffer* buffer = 0;

	switch (texUsage)
	{
	case eTextureUsageInputBuffer:
		hr = RenderDevice::This().GetDevice()->CreateBuffer(&Desc, &input, &buffer);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
		srvDesc.BufferEx.FirstElement = 0;
		srvDesc.BufferEx.Flags = 0;
		srvDesc.BufferEx.NumElements = elementCount;

		hr = RenderDevice::This().GetDevice()->CreateShaderResourceView(buffer, &srvDesc, &pTexture->m_pResourceView);
		break;
	case eTextureUsageOutputBuffer:
		hr = RenderDevice::This().GetDevice()->CreateBuffer(&Desc, 0, &pTexture->m_pOutputBuffer);

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.Flags = 0;
		uavDesc.Buffer.NumElements = elementCount;

		hr = RenderDevice::This().GetDevice()->CreateUnorderedAccessView(pTexture->m_pOutputBuffer, &uavDesc, &pTexture->m_pUnorderdView);
		break;
	}

	return pTexture;
}

void TextureManager::ReleaseTexture(Texture * pTexture)
{

}

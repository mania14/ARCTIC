#pragma once
#include "../System/RenderDevice.h"
#include "../System/DDSTextureLoader.h"
#include "../System/WICTextureLoader.h"
#include "FBXUtill.h"

class Texture;

enum TextureUsage
{
	eTextureUsageDefault,
	eTextureUsageInputBuffer,
	eTextureUsageOutputBuffer,
	eTextureUsageCount
};

class TextureManager : public Singletone<TextureManager>
{
public:
	void	 SaveTextureFile(Texture* pTex);
	Texture* LoadTexture(std::string strFileName);
	Texture* LoadTexture2DArray(std::string textureSetName, std::vector<std::string>& filenames);

	Texture* CreateFBXTexture(const std::string& meshName, FbxNode* pNode);
	Texture* CreateRenderTargetTexture(UINT width, UINT height, DXGI_FORMAT format, UINT BindFlags);
	Texture* CreateNoiseTexture(UINT width, UINT height);

	template<class BufferType>
	Texture* CreateInputBufferTexture(BufferType* pBuffer, UINT elementCount)
	{
		return CreateBufferTexture(eTextureUsageInputBuffer, pBuffer, sizeof(BufferType), elementCount);
	};
	
	template<class BufferType>
	Texture* CreateOutputBufferTexture(BufferType* pBuffer, UINT elementCount)
	{
		return CreateBufferTexture(eTextureUsageOutputBuffer, pBuffer, sizeof(BufferType), elementCount);
	};

	void	 ReleaseTexture(Texture* pTexture);

private:
	template<class BufferType>
	void ReadBufferTexture(Texture* pTexture, BufferType* pBuffer, UINT elementCount)
	{
		HRESULT hr;
		D3D11_BUFFER_DESC outputDesc;
		outputDesc.Usage = D3D11_USAGE_STAGING;
		outputDesc.BindFlags = 0;
		outputDesc.ByteWidth = sizeof(BufferType) * elementCount;
		outputDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		outputDesc.StructureByteStride = sizeof(BufferType);
		outputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		ID3D11Buffer* OutputBuffer;
		hr = RenderDevice::This().GetDevice()->CreateBuffer(&outputDesc, 0, &OutputBuffer);

		RenderDevice::This().GetContext()->CopyResource(OutputBuffer, pTexture->m_pOutputBuffer);

		D3D11_MAPPED_SUBRESOURCE mappedData;
		hr = RenderDevice::This().GetContext()->Map(OutputBuffer, 0, D3D11_MAP_READ, 0, &mappedData);
		BufferType* tempBuffer = reinterpret_cast<BufferType*>(mappedData.pData);

		memcpy(pBuffer, tempBuffer, sizeof(BufferType) * elementCount);
		//*pBuffer = *tempBuffer;

		//사용후 호출
		RenderDevice::This().GetContext()->Unmap(OutputBuffer, 0);
	};

private:
	Texture* CreateBufferTexture(TextureUsage texUsage, void* pvBuffer, UINT sizeofBufferType, UINT elementCount);
};
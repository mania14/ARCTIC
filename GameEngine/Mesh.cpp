#include "stdafx.h"
#include "../System/RenderDevice.h"
#include "Mesh.h"
#include "directxcolors.h"
#include "../System/ConstBufferDesc.h"

using namespace DirectX;

Mesh::Mesh()
{
	ZeroMemory(pTexture, sizeof(Texture*) * TEXTURE_TYPE_COUNT);
}

Mesh::~Mesh()
{
}

int Mesh::Init()
{
	//���׸���
	pMaterial = new Material();
	pMaterial->Ambient = XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	pMaterial->Diffuse = XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	pMaterial->Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);
	pMaterial->Reflect = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	return false;
}

void Mesh::Draw()
{
	UINT stride = GetVertexSize();
	UINT offset = 0;

	//Marterial
	//RenderDevice::This().GetRawVariableByName("gMaterial")->SetRawValue(pMaterial, 0, sizeof(Material));

	//Texture
	if(nullptr != pTexture[Mesh::TEXTURE_TYPE_DIFFUSE])
	{
		RenderDevice::This().GetRawVariableByName("gDiffuseMap")->AsShaderResource()->SetResource(pTexture[Mesh::TEXTURE_TYPE_DIFFUSE]->m_pResourceView);
	}

	if (nullptr != pTexture[Mesh::TEXTURE_TYPE_NORMAL])
	{
		RenderDevice::This().GetRawVariableByName("gNormalMap")->AsShaderResource()->SetResource(pTexture[Mesh::TEXTURE_TYPE_NORMAL]->m_pResourceView);
	}

	if( GetMeshBuffer()->vBuffer != nullptr && GetMeshBuffer()->vIBuffer != nullptr )
	{
		RenderDevice::This().GetPassByIndex(0)->Apply(0, RenderDevice::This().GetContext());
		RenderDevice::This().GetContext()->IASetVertexBuffers(0, 1, &GetMeshBuffer()->vBuffer, &stride, &offset);
		RenderDevice::This().GetContext()->IASetIndexBuffer(GetMeshBuffer()->vIBuffer, DXGI_FORMAT_R32_UINT, 0);
		RenderDevice::This().GetContext()->DrawIndexed((int)GetIndicsCount(), 0, 0);
	}

	for (auto iter = vecChildMesh.begin(); iter != vecChildMesh.end(); ++iter)
	{
		(*iter)->Draw();
	}
}

int Mesh::Release()
{
	if(nullptr != pMeshBuffer.vBuffer)
	{
		pMeshBuffer.vBuffer->Release();
	}

	if(nullptr != pMeshBuffer.vIBuffer)
	{
		pMeshBuffer.vIBuffer->Release();
	}

	if(nullptr != pMaterial)
	{
		delete pMaterial;
	}

	for (auto iter = vecChildMesh.begin(); iter != vecChildMesh.end(); ++iter)
	{
		(*iter)->Release();
		delete (*iter);
	}

	return true;
}
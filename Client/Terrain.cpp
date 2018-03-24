#include "..\GameEngine\Terrain.h"
#include "..\GameEngine\Terrain.h"
#include "stdafx.h"
#include "Terrain.h"
#include "../GameEngine/Transform.h"
#include "../System/RenderDevice.h"
#include "../GameEngine/CameraManager.h"
#include "../GameEngine/MeshFactory.h"
#include "../GameEngine/Mesh.h"
#include "../GameEngine/TextureManager.h"

//지우자
#include "../GameEngine/LightManager.h"
#include "../System/ConstBufferDesc.h"

Terrain::Terrain()
{
}

Terrain::~Terrain()
{
	Destroy();
}

void Terrain::Create()
{
	//Transform
	Transform* pTransform = new Transform();
	pTransform->Init();
	AddComponent(pTransform);

	//메쉬
	Mesh* pMesh = new Mesh();
	MeshFactory::This().CreateGrid(128, 128, 0, *pMesh);
	AddComponent(pMesh);

	//텍스쳐
	Texture* pTexture = TextureManager::This().LoadTexture("../Res/Texture/Test.dds");
	AddComponent(pTexture);

}

int Terrain::Init()
{
	return 0;
}

int Terrain::Release()
{
	return 0;
}

int Terrain::Update()
{
	GameObject::Update();
}

void Terrain::Render()
{
	Mesh* pMesh = GetComponent<Mesh>();
	Transform* pTransform = GetComponent<Transform>();
	//Texture* pTexture = GetComponent<Texture>();

	XMMATRIX worldViewProj = pTransform->GetWorldMatrix() * CameraManager::This().GetCurrentCameraViewProj();
	XMMATRIX world = pTransform->GetWorldMatrix();
	XMMATRIX worldInvTranspose = pTransform->GetWorldMatrixInvTranspose();


	RenderDevice::This().Begin("PackGBufferTech");
	{
		UINT stride = pMesh->GetVertexSize();
		UINT offset = 0;
			
		//빛
		//LightManager::This().Update();

		//위치행렬
		HRESULT hr;
		hr = RenderDevice::This().GetVariableByName("gWorldViewProj")->SetMatrix(reinterpret_cast<float*>(&worldViewProj));
		hr = RenderDevice::This().GetVariableByName("gWorld")->SetMatrix(reinterpret_cast<float*>(&world));
		hr = RenderDevice::This().GetVariableByName("gWorldInvTranspose")->SetMatrix(reinterpret_cast<float*>(&worldInvTranspose));

		//마테리얼
		Material mLandMat;
		mLandMat.Ambient = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
		mLandMat.Diffuse = XMFLOAT4(0.48f, 0.77f, 0.46f, 0.2f);
		mLandMat.Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);
		mLandMat.Reflect = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		RenderDevice::This().GetRawVariableByName("gMaterial")->SetRawValue(&mLandMat, 0, sizeof(mLandMat));

		//텍스쳐		
		//RenderDevice::This().GetRawVariableByName("gDiffuseMap")->AsShaderResource()->SetResource(pTexture[Texture::TEXTURE_TYPE_DIFFUSE]->m_pResourceView);

		RenderDevice::This().GetPassByIndex(0)->Apply(0, RenderDevice::This().GetContext());

		RenderDevice::This().GetContext()->IASetVertexBuffers(0, 1, &pMesh->GetMeshBuffer()->vBuffer, &stride, &offset);
		RenderDevice::This().GetContext()->IASetIndexBuffer(pMesh->GetMeshBuffer()->vIBuffer, DXGI_FORMAT_R32_UINT, 0);
		RenderDevice::This().GetContext()->DrawIndexed((int)pMesh->GetIndicsCount(), 0, 0);
	}
	RenderDevice::This().End("PackGBufferTech");
}

void Terrain::Destroy()
{
	GameObject::Destroy();
}
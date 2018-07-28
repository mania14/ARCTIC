#include "stdafx.h"
#include "SkyBox.h"
#include "../GameEngine/Transform.h"
#include "../System/RenderDevice.h"
#include "../GameEngine/CameraManager.h"
#include "../GameEngine/MeshFactory.h"
#include "../GameEngine/Mesh.h"
#include "../GameEngine/TextureManager.h"
#include "../GameEngine/Texture.h"

//지우자
#include "../GameEngine/LightManager.h"
#include "../System/ConstBufferDesc.h"

SkyBox::SkyBox()
{
}

SkyBox::~SkyBox()
{
	Destroy();
}

void SkyBox::Create()
{
	//TextureManager::This().CreateNoiseTexture(64, 64);

	//Transform
	Transform* pTransform = new Transform();
	pTransform->Init();
	AddComponent(pTransform);

	//메쉬
	Mesh* pMesh = new Mesh();
	MeshFactory::This().CreateGeoSphere(1, 2, *pMesh);
	AddComponent(pMesh);

	//텍스쳐
	Texture* pTexture = TextureManager::This().LoadTexture("../Res/Texture/SkyBox.dds");
	AddComponent(pTexture);

}

void SkyBox::Update()
{
	GameObject::Update();
}

void SkyBox::Render()
{
	Mesh* pMesh = GetComponent<Mesh>();
	Transform* pTransform = GetComponent<Transform>();
	Texture* pTexture = GetComponent<Texture>();

	float4x4 rotateView = CameraManager::This().GetCurrentCameraView();
	rotateView._41 = 0.f; rotateView._42 = 0.f; rotateView._43 = 0.f; rotateView._44 = 1.f;

	float4x4 worldViewProj = rotateView * CameraManager::This().GetCurrentCameraProj();


	RenderDevice::This().Begin("SkyTech");
	{
		UINT stride = pMesh->GetVertexSize();
		UINT offset = 0;

		//위치행렬
		HRESULT hr;
		hr = RenderDevice::This().GetVariableByName("gWorldViewProj")->SetMatrix(reinterpret_cast<float*>(&worldViewProj));

		//텍스쳐		
		hr = RenderDevice::This().GetRawVariableByName("gCubeMap")->AsShaderResource()->SetResource(pTexture->m_pResourceView);

		RenderDevice::This().GetPassByIndex(0)->Apply(0, RenderDevice::This().GetContext());

		RenderDevice::This().GetContext()->IASetVertexBuffers(0, 1, &pMesh->GetMeshBuffer()->vBuffer, &stride, &offset);
		RenderDevice::This().GetContext()->IASetIndexBuffer(pMesh->GetMeshBuffer()->vIBuffer, DXGI_FORMAT_R32_UINT, 0);
		RenderDevice::This().GetContext()->DrawIndexed((int)pMesh->GetIndicsCount(), 0, 0);
	}
	RenderDevice::This().End("SkyTech");

}

void SkyBox::Destroy()
{
	GameObject::Destroy();
}
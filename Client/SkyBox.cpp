#include "stdafx.h"
#include "SkyBox.h"
#include "../GameEngine/Transform.h"
#include "../System/RenderDevice.h"
#include "../GameEngine/CameraManager.h"
#include "../GameEngine/MeshFactory.h"
#include "../GameEngine/Mesh.h"
#include "../GameEngine/TextureManager.h"

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
	pMesh->SetTexture(Mesh::TEXTURE_SLOT0, TextureManager::This().LoadTexture("../Res/Texture/SkyBox.dds"));

	AddComponent(pMesh);
}

void SkyBox::Update()
{
	GameObject::Update();
}

void SkyBox::Render()
{
	Mesh* pMesh = GetComponent<Mesh>();
	Transform* pTransform = GetComponent<Transform>();

	float4x4 rotateView = CameraManager::This().GetCurrentCameraView();
	rotateView._41 = 0.f; rotateView._42 = 0.f; rotateView._43 = 0.f; rotateView._44 = 1.f;

	float4x4 worldViewProj = rotateView * CameraManager::This().GetCurrentCameraProj();


	RenderDevice::This().BeginFX("SkyTech");
	{
		UINT stride = pMesh->GetVertexSize();
		UINT offset = 0;

		//위치행렬
		HRESULT hr;
		hr = RenderDevice::This().GetVariableByName("gWorldViewProj")->SetMatrix(reinterpret_cast<float*>(&worldViewProj));

		//텍스쳐		
		hr = RenderDevice::This().GetRawVariableByName("gCubeMap")->AsShaderResource()->SetResource(pMesh->GetTexture(Mesh::TEXTURE_SLOT0)->m_pResourceView);

		RenderDevice::This().ApplyFX();

		RenderDevice::This().GetContext()->IASetVertexBuffers(0, 1, &pMesh->GetMeshBuffer()->vBuffer, &stride, &offset);
		RenderDevice::This().GetContext()->IASetIndexBuffer(pMesh->GetMeshBuffer()->vIBuffer, DXGI_FORMAT_R32_UINT, 0);
		RenderDevice::This().GetContext()->DrawIndexed((int)pMesh->GetIndicsCount(), 0, 0);

		RenderDevice::This().GetRawVariableByName("gCubeMap")->AsShaderResource()->SetResource(nullptr);
	}
	RenderDevice::This().EndFX();

}

void SkyBox::Destroy()
{
	GameObject::Destroy();
}
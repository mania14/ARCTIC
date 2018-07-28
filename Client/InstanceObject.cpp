#include "stdafx.h"
#include "InstanceObject.h"
#include "../GameEngine/Transform.h"
#include "../System/RenderDevice.h"
#include "../GameEngine/CameraManager.h"
#include "../GameEngine/MeshFactory.h"
#include "../GameEngine/Mesh.h"
#include "../GameEngine/Instancing.h"
#include "../GameEngine/TextureManager.h"

//지우자
#include "../GameEngine/LightManager.h"
#include "../System/ConstBufferDesc.h"

InstanceObject::InstanceObject()
{
}

InstanceObject::~InstanceObject()
{
	Destroy();
}

void InstanceObject::Create()
{
	//Transform
	Transform* pTransform = new Transform();
	pTransform->Init();
	AddComponent(pTransform);

	//메쉬
	Mesh* pMesh = new Mesh();
	MeshFactory::This().CreateBox(*pMesh);
	AddComponent(pMesh);

	Instancing* pInstance = new Instancing();
	MeshFactory::This().CreateInstance(*pMesh, *pInstance);
	AddComponent(pInstance);

	//텍스쳐
	Texture* pTexture = TextureManager::This().LoadTexture("../Res/Texture/WireFence.dds");
	AddComponent(pTexture);

}

void InstanceObject::Update()
{
	GameObject::Update();
}

void InstanceObject::Render()
{
	//인스턴스는 따로 컴포넌트 속성으로 렌더하자


	Mesh* pMesh = GetComponent<Mesh>();
	Transform* pTransform = GetComponent<Transform>();
	Texture* pTexture = GetComponent<Texture>();
	Instancing* pInstancing = GetComponent<Instancing>();

	float4x4 worldViewProj = pTransform->GetWorldMatrix() * CameraManager::This().GetCurrentCameraViewProj();
	float4x4 world = pTransform->GetWorldMatrix();
	float4x4 worldInvTranspose = world;
	worldInvTranspose.inverse();
	worldInvTranspose.transpose();


	RenderDevice::This().Begin("defaultlight");
	{
		//빛
		//LightManager::This().Update();

		//위치행렬
		HRESULT hr;
		hr = RenderDevice::This().GetVariableByName("gWorldViewProj")->SetMatrix(reinterpret_cast<float*>(&worldViewProj));
		hr = RenderDevice::This().GetVariableByName("gWorld")->SetMatrix(reinterpret_cast<float*>(&world));
		hr = RenderDevice::This().GetVariableByName("gWorldInvTranspose")->SetMatrix(reinterpret_cast<float*>(&worldInvTranspose));

		//마테리얼
		Material mLandMat;
		mLandMat.Ambient = float4(0.48f, 0.77f, 0.46f, 1.0f);
		mLandMat.Diffuse = float4(0.48f, 0.77f, 0.46f, 1.0f);
		mLandMat.Specular = float4(0.2f, 0.2f, 0.2f, 16.0f);
		mLandMat.Reflect = float4(0.0f, 0.0f, 0.0f, 0.0f);
		RenderDevice::This().GetRawVariableByName("gMaterial")->SetRawValue(&mLandMat, 0, sizeof(mLandMat));

		//텍스쳐		
		//hr = RenderDevice::This().GetRawVariableByName("gDiffuseMap")->AsShaderResource()->SetResource(pTexture->m_pResourceView);

		RenderDevice::This().GetPassByIndex(0)->Apply(0, RenderDevice::This().GetContext());

		//RenderDevice::This().GetContext()->IASetVertexBuffers(0, 1, &pMesh->GetMeshBuffer()->vBuffer, &stride, &offset);
		//RenderDevice::This().GetContext()->IASetIndexBuffer(pMesh->GetMeshBuffer()->vIBuffer, DXGI_FORMAT_R32_UINT, 0);

		//RenderDevice::This().GetContext()->IASetVertexBuffers(0, 1, &pMesh->GetMeshBuffer()->vBuffer, &stride, &offset);
		//RenderDevice::This().GetContext()->DrawIndexed((int)pMesh->GetIndicsCount(), 0, 0);

		UINT stride[2] = { (UINT)pMesh->GetVertexSize(), (UINT)pInstancing->GetVertexSize() };
		UINT offset[2] = { 0,0 };
		ID3D11Buffer* vBuffers[2] = { pMesh->GetMeshBuffer()->vBuffer, pInstancing->GetBuffer()};

		RenderDevice::This().GetContext()->IASetVertexBuffers(0, 2, vBuffers, stride, offset);
		RenderDevice::This().GetContext()->IASetIndexBuffer(pMesh->GetMeshBuffer()->vIBuffer, DXGI_FORMAT_R32_UINT, 0);
		RenderDevice::This().GetContext()->DrawIndexedInstanced((int)pMesh->GetIndicsCount(), 100, 0, 0, 0);
	}
	RenderDevice::This().End("deafaultlight");

}

void InstanceObject::Destroy()
{
	GameObject::Destroy();
}
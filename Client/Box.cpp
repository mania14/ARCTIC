#include "stdafx.h"
#include "Box.h"
#include "../GameEngine/Transform.h"
#include "../System/RenderDevice.h"
#include "../GameEngine/CameraManager.h"
#include "../GameEngine/MeshFactory.h"
#include "../GameEngine/Mesh.h"
#include "../GameEngine/TextureManager.h"
#include "../GameEngine/Collision.h"
#include "../GameEngine/FBXUtill.h"
#include "../GameEngine/CollisionFactory.h"

//지우자
#include "../GameEngine/LightManager.h"
#include "../System/ConstBufferDesc.h"

Box::Box()
{
}

Box::~Box()
{
	Destroy();
}

void Box::Create()
{
	//Transform
	Transform* pTransform = new Transform();
	pTransform->Init();
	AddComponent(pTransform);

	//Mesh
	Mesh* pMesh = new Mesh();
	MeshFactory::This().CreateGeoSphere(3, 4, *pMesh);
	AddComponent(pMesh);

	//FBXUtill
	
	//메쉬
	//FBXUtill::This().Init();
	//Mesh* pMesh = new Mesh();
	////MeshFactory::This().CreateFBXMesh("D:\\Project\\DX11\\ARCTIC\\Res\\Mesh\\GLaDOS\\1.fbx", *pMesh);
	////MeshFactory::This().CreateFBXMesh("D:\\Project\\DX11\\ARCTIC\\Res\\Mesh\\inc\\MI.fbx", *pMesh);
	//MeshFactory::This().CreateFBXMesh("D:\\Project\\DX11\\ARCTIC\\Res\\Mesh\\R\\Rocket.fbx", *pMesh);
	////MeshFactory::This().CreateFBXMesh("D:\\Project\\DX11\\ARCTIC\\Res\\Mesh\\mega\\megalodon.fbx", *pMesh);
	//AddComponent(pMesh);
	
	//Collision* pCollision = new Collision();
	//CollisionFactory::This().CreateTriangleMeshCollision(*pMesh, pCollision);
	//AddComponent(pCollision);

	//텍스쳐
	//Texture* pTexture= TextureManager::This().CreateFBXTexture(pScene->Get);
	//AddComponent(pTexture);

	//Texture* pCubeTexture = TextureManager::This().LoadTexture("../Res/Texture/SkyBox.dds");
	//AddComponent(pCubeTexture);

	pMesh->pTexture[Mesh::TEXTURE_SLOT0] = TextureManager::This().LoadTexture("../Res/Texture/Sphere.dds");
	pMesh->pTexture[Mesh::TEXTURE_SLOT1] = TextureManager::This().LoadTexture("../Res/Texture/normal2.dds");
}

void Box::Update()
{
	GameObject::Update();
}

void Box::Render()
{
	Mesh* pMesh = GetComponent<Mesh>();
	Transform* pTransform = GetComponent<Transform>();
		
	float4x4 worldViewProj = pTransform->GetWorldMatrix() * CameraManager::This().GetCurrentCameraViewProj();
	float4x4 world = pTransform->GetWorldMatrix();
	
	RenderDevice::This().BeginFX("PackGBufferTech");
	{	
		HRESULT hr;
		hr = RenderDevice::This().GetVariableByName("gWorldViewProj")->SetMatrix(reinterpret_cast<float*>(&worldViewProj));
		hr = RenderDevice::This().GetVariableByName("gWorld")->SetMatrix(reinterpret_cast<float*>(&world));

		RenderDevice::This().GetPassByIndex(0)->Apply(0, RenderDevice::This().GetContext());
		pMesh->Draw();
	}
	RenderDevice::This().EndFX();

	pTransform->RenderBox();
	
}

void Box::Destroy()
{
	GameObject::Destroy();
}
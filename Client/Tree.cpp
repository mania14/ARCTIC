#include "stdafx.h"
#include "Tree.h"
#include "../GameEngine/Transform.h"
#include "../System/RenderDevice.h"
#include "../GameEngine/CameraManager.h"
#include "../GameEngine/MeshFactory.h"
#include "../GameEngine/Billboard.h"
#include "../GameEngine/TextureManager.h"
#include "../System/Texture.h"
#include "../System/RenderState.h"

//������
#include "../GameEngine/LightManager.h"
#include "../System/ConstBufferDesc.h"

Tree::Tree()
{
}

Tree::~Tree()
{
	Destroy();
}

void Tree::Create()
{
	//Transform
	Transform* pTransform = new Transform();
	pTransform->Init();
	AddComponent(pTransform);

	//�޽�
	Billboard* pBillboard = new Billboard();

	std::vector<Vertex_Billboard> vecData;
	for (int i = 0; i < 50; ++i)
	{
		Vertex_Billboard pData;
		pData.Pos = float3((float)i * 10, 20, (float)i * 10);
		pData.Size = float2(20, 20);
		vecData.push_back(pData);
	}

	MeshFactory::This().CreateBillboard(*pBillboard, vecData);
	AddComponent(pBillboard);

	//�ؽ���
	//Texture* pTexture = TextureManager::This().LoadTexture("../Res/Texture/Test.dds");

	//std::vector<std::string> filenames
	//{
	//	"../Res/Texture/tree0.dds",
	//	"../Res/Texture/tree1.dds",
	//	"../Res/Texture/tree2.dds",
	//	"../Res/Texture/tree3.dds"
	//};

	//Texture* pTexture = TextureManager::This().LoadTexture2DArray("Tree", filenames);
	//AddComponent(pTexture);

}

void Tree::Update()
{
	GameObject::Update();
}

void Tree::Render()
{
	Billboard* pBillboard = GetComponent<Billboard>();
	Transform* pTransform = GetComponent<Transform>();
	//Texture* pTexture = GetComponent<Texture>();

	float4x4 ViewProj = CameraManager::This().GetCurrentCameraViewProj();

	RenderDevice::This().BeginFX("LightBillboardTech");
	{
		RenderDevice::This().GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		UINT stride = pBillboard->GetVertexSize();
		UINT offset = 0;

		//��
		//LightManager::This().Update();

		//��ġ���
		HRESULT hr;
		hr = RenderDevice::This().GetVariableByName("gViewProj")->SetMatrix(reinterpret_cast<float*>(&ViewProj));

		//���׸���
		Material mLandMat;
		mLandMat.Ambient = float4(0.48f, 0.77f, 0.46f, 1.0f);
		mLandMat.Diffuse = float4(0.48f, 0.77f, 0.46f, 1.0f);
		mLandMat.Specular = float4(0.2f, 0.2f, 0.2f, 16.0f);
		mLandMat.Reflect = float4(0.0f, 0.0f, 0.0f, 0.0f);
		RenderDevice::This().GetRawVariableByName("gMaterial")->SetRawValue(&mLandMat, 0, sizeof(mLandMat));

		//�ؽ���		
		//hr = RenderDevice::This().GetRawVariableByName("gDiffuseMap")->AsShaderResource()->SetResource(pTexture->m_pResourceView);

		RenderDevice::This().ApplyFX();

		//���� ������
		RenderDevice::This().GetContext()->OMSetBlendState(RenderStates::This().AlphaToCoverageBS, blendFactor, 0xffffffff);

		ID3D11Buffer* pBuffer = pBillboard->GetBillboardBuffer();
		RenderDevice::This().GetContext()->IASetVertexBuffers(0, 1, &pBuffer, &stride, &offset);
		RenderDevice::This().GetContext()->Draw((int)pBillboard->GetVertexCount(), 0);
	}
	RenderDevice::This().EndFX();

}

void Tree::Destroy()
{
	GameObject::Destroy();
}
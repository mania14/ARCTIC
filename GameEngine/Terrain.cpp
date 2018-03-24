#include "stdafx.h"
#include "Terrain.h"
#include "../System/RenderDevice.h"
#include "../System/VertexDesc.h"
#include "CameraManager.h"

Terrain::Terrain()
{
}

Terrain::~Terrain()
{
	Release();
}

int Terrain::Init()
{
	for (int i = 0; i < 1; i++)
	{
		Vertex_VoxelTerrain_Instance* vterrain = new Vertex_VoxelTerrain_Instance();
		//vterrain->PositionIndex = XMFLOAT4(i, 0, 0, i);
		vterrain->Density0 = XMFLOAT4(1, 1, 1, 1);
		vterrain->Density1 = XMFLOAT4(1, 1, 1, 1);
		vterrain->Normal0 = XMFLOAT3(1, 0, 0);
		vterrain->Normal1 = XMFLOAT3(0, 1, 0);
		vterrain->Normal2 = XMFLOAT3(0, 0, 1);
		vterrain->Normal3 = XMFLOAT3(1, 1, 0);
		vterrain->Normal4 = XMFLOAT3(0, 1, 1);
		vterrain->Normal5 = XMFLOAT3(1, 0, 1);
		vterrain->Normal6 = XMFLOAT3(0, 1, 0);
		vterrain->Normal7 = XMFLOAT3(0, 1, 0);
		_vecTerrainVoxel.push_back(vterrain);
	}

	{
		D3D11_BUFFER_DESC vBufferDesc;
		vBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vBufferDesc.ByteWidth = sizeof(Vertex_VoxelTerrain_Instance) * 1;
		vBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vBufferDesc.CPUAccessFlags = 0;
		vBufferDesc.MiscFlags = 0;
		vBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vInitData;
		vInitData.pSysMem = &_vecTerrainVoxel[0];

		HR(RenderDevice::This().GetDevice()->CreateBuffer(&vBufferDesc, &vInitData, &_vInstanceBuffer), L"instance buffer create fail");
	}


	// Create Vertex Buffer
	std::vector<Vertex_Debug> temp;
	Vertex_Debug debug;
	debug.Pos = XMFLOAT3(1, 1, 1);
	temp.push_back(debug);

	{
		D3D11_BUFFER_DESC vBufferDesc;
		vBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vBufferDesc.ByteWidth = sizeof(Vertex_Debug);
		vBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vBufferDesc.CPUAccessFlags = 0;
		vBufferDesc.MiscFlags = 0;
		vBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vInitData;
		vInitData.pSysMem = &temp[0];

		HR(RenderDevice::This().GetDevice()->CreateBuffer(&vBufferDesc, &vInitData, &_vTempBuffer), L"test buffer create fail");
	}

	return true;
}

int Terrain::Update()
{
	return 0;
}

void Terrain::TestRender()
{
	RenderDevice::This().Begin("TerrainTech");
	{
		UINT stride[2] = { sizeof(Vertex_Debug), sizeof(Vertex_VoxelTerrain_Instance) };
		UINT offset[2] = { 0,0 };
		ID3D11Buffer* vBuffers[2] = { _vTempBuffer, _vInstanceBuffer };

		XMMATRIX worldViewProj = CameraManager::This().GetCurrentCameraViewProj();
		RenderDevice::This().GetVariableByName("gWorldViewProj")->SetMatrix(reinterpret_cast<float*>(&worldViewProj));
		RenderDevice::This().GetPassByIndex(0)->Apply(0, RenderDevice::This().GetContext());

		RenderDevice::This().GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		RenderDevice::This().GetContext()->IASetVertexBuffers(0, 2, vBuffers, stride, offset);

		RenderDevice::This().GetContext()->DrawInstanced(1, 1, 0, 0);
	}
	RenderDevice::This().End("TerrainTech");
}

int Terrain::Release()
{
	return true;
}
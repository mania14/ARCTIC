#include "stdafx.h"
#include "../System/RenderDevice.h"
#include "../CommonUtil/Math.h"
#include "MeshFactory.h"
#include "Mesh.h"
#include "Billboard.h"
#include "directxcolors.h"
#include "TextureManager.h"
#include "../CommonUtil/Macro.h"
#include "../System/FileSystem.h"
#include "TextureManager.h"
#include <functional>

MeshFactory::MeshFactory()
{
	Init();
}

MeshFactory::~MeshFactory()
{
	Release();
}

void MeshFactory::Init()
{
	
}

void MeshFactory::Release()
{
}

bool MeshFactory::CreateTerrainVoxel(MeshBuffer& meshBuffer, std::vector<Vertex_Voxel_Terrain>& _data, std::vector<UINT>& _index)
{
	//정점 버퍼 생성

	D3D11_BUFFER_DESC vBufferDesc;
	vBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vBufferDesc.ByteWidth = sizeof(Vertex_Voxel_Terrain) * _data.size();
	vBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vBufferDesc.CPUAccessFlags = 0;
	vBufferDesc.MiscFlags = 0;
	vBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vInitData;
	vInitData.pSysMem = &_data[0];

	if (FAILED(RenderDevice::This().GetDevice()->CreateBuffer(&vBufferDesc, &vInitData, &meshBuffer.vBuffer)))
	{
		return false;
	}

	//create buffer indices;

	D3D11_BUFFER_DESC vIdxBufferDesc;
	vIdxBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vIdxBufferDesc.ByteWidth = sizeof(UINT) * (UINT)_index.size();
	vIdxBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	vIdxBufferDesc.CPUAccessFlags = 0;
	vIdxBufferDesc.MiscFlags = 0;
	vIdxBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA iIdxInitData;
	iIdxInitData.pSysMem = &_index[0];

	if(FAILED(RenderDevice::This().GetDevice()->CreateBuffer(&vIdxBufferDesc, &iIdxInitData, &meshBuffer.vIBuffer), L"CreateBuffer Index Mesh Buffer Fail"));
	{
		return false;
	}
}

bool MeshFactory::CreateBox(Mesh & meshData)
{
	//정점 버퍼 생성

	meshData.Init();

	meshData.vertexSize = sizeof(Vertex_Tex);

	float w2 = 0.5f*1;
	float h2 = 0.5f*1;
	float d2 = 0.5f*1;

	std::vector<Vertex_Tex> vertices =
	{
		/*	{ acm::float3(-w2, -h2, -d2), acm::float3(0.0f, 0.0f, -1.0f), acm::float3(1.0f, 0.0f, 0.0f), acm::float2(0.0f, 1.0f) },
			{ acm::float3(-w2, +h2, -d2), acm::float3(0.0f, 0.0f, -1.0f), acm::float3(1.0f, 0.0f, 0.0f), acm::float2(0.0f, 0.0f) },
			{ acm::float3(+w2, +h2, -d2), acm::float3(0.0f, 0.0f, -1.0f), acm::float3(1.0f, 0.0f, 0.0f), acm::float2(1.0f, 0.0f) },
			{ acm::float3(+w2, -h2, -d2), acm::float3(0.0f, 0.0f, -1.0f), acm::float3(1.0f, 0.0f, 0.0f), acm::float2(1.0f, 1.0f) },

			{ acm::float3(-w2, -h2, +d2), acm::float3(0.0f, 0.0f, 1.0f), acm::float3(-1.0f, 0.0f, 0.0f), acm::float2(1.0f, 1.0f) },
			{ acm::float3(+w2, -h2, +d2), acm::float3(0.0f, 0.0f, 1.0f), acm::float3(-1.0f, 0.0f, 0.0f), acm::float2(0.0f, 1.0f) },
			{ acm::float3(+w2, +h2, +d2), acm::float3(0.0f, 0.0f, 1.0f), acm::float3(-1.0f, 0.0f, 0.0f), acm::float2(0.0f, 0.0f) },
			{ acm::float3(-w2, +h2, +d2), acm::float3(0.0f, 0.0f, 1.0f), acm::float3(-1.0f, 0.0f, 0.0f), acm::float2(1.0f, 0.0f) },

			{ acm::float3(-w2, +h2, -d2), acm::float3(0.0f, 1.0f, 0.0f), acm::float3(1.0f, 0.0f, 0.0f), acm::float2(0.0f, 1.0f) },
			{ acm::float3(-w2, +h2, +d2), acm::float3(0.0f, 1.0f, 0.0f), acm::float3(1.0f, 0.0f, 0.0f), acm::float2(0.0f, 0.0f) },
			{ acm::float3(+w2, +h2, +d2), acm::float3(0.0f, 1.0f, 0.0f), acm::float3(1.0f, 0.0f, 0.0f), acm::float2(1.0f, 0.0f) },
			{ acm::float3(+w2, +h2, -d2), acm::float3(0.0f, 1.0f, 0.0f), acm::float3(1.0f, 0.0f, 0.0f), acm::float2(1.0f, 1.0f) },

			{ acm::float3(-w2, -h2, -d2), acm::float3(0.0f, -1.0f, 0.0f), acm::float3(-1.0f, 0.0f, 0.0f), acm::float2(1.0f, 1.0f) },
			{ acm::float3(+w2, -h2, -d2), acm::float3(0.0f, -1.0f, 0.0f), acm::float3(-1.0f, 0.0f, 0.0f), acm::float2(0.0f, 1.0f) },
			{ acm::float3(+w2, -h2, +d2), acm::float3(0.0f, -1.0f, 0.0f), acm::float3(-1.0f, 0.0f, 0.0f), acm::float2(0.0f, 0.0f) },
			{ acm::float3(-w2, -h2, +d2), acm::float3(0.0f, -1.0f, 0.0f), acm::float3(-1.0f, 0.0f, 0.0f), acm::float2(1.0f, 0.0f) },

			{ acm::float3(-w2, -h2, +d2), acm::float3(-1.0f, 0.0f, 0.0f), acm::float3(0.0f, 0.0f, -1.0f), acm::float2(0.0f, 1.0f) },
			{ acm::float3(-w2, +h2, +d2), acm::float3(-1.0f, 0.0f, 0.0f), acm::float3(0.0f, 0.0f, -1.0f), acm::float2(0.0f, 0.0f) },
			{ acm::float3(-w2, +h2, -d2), acm::float3(-1.0f, 0.0f, 0.0f), acm::float3(0.0f, 0.0f, -1.0f), acm::float2(1.0f, 0.0f) },
			{ acm::float3(-w2, -h2, -d2), acm::float3(-1.0f, 0.0f, 0.0f), acm::float3(0.0f, 0.0f, -1.0f), acm::float2(1.0f, 1.0f) },

			{ acm::float3(+w2, -h2, -d2), acm::float3(1.0f, 0.0f, 0.0f), acm::float3(0.0f, 0.0f, 1.0f), acm::float2(0.0f, 1.0f) },
			{ acm::float3(+w2, +h2, -d2), acm::float3(1.0f, 0.0f, 0.0f), acm::float3(0.0f, 0.0f, 1.0f), acm::float2(0.0f, 0.0f) },
			{ acm::float3(+w2, +h2, +d2), acm::float3(1.0f, 0.0f, 0.0f), acm::float3(0.0f, 0.0f, 1.0f), acm::float2(1.0f, 0.0f) },
			{ acm::float3(+w2, -h2, +d2), acm::float3(1.0f, 0.0f, 0.0f), acm::float3(0.0f, 0.0f, 1.0f), acm::float2(1.0f, 1.0f) },*/
	};

	//인덱스 버퍼 생성
	std::vector<UINT> indices;
	indices.resize(36);

	// Fill in the front face index data
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	// Fill in the back face index data
	indices[6] = 4; indices[7] = 5; indices[8] = 6;
	indices[9] = 4; indices[10] = 6; indices[11] = 7;

	// Fill in the top face index data
	indices[12] = 8; indices[13] = 9; indices[14] = 10;
	indices[15] = 8; indices[16] = 10; indices[17] = 11;

	// Fill in the bottom face index data
	indices[18] = 12; indices[19] = 13; indices[20] = 14;
	indices[21] = 12; indices[22] = 14; indices[23] = 15;

	// Fill in the left face index data
	indices[24] = 16; indices[25] = 17; indices[26] = 18;
	indices[27] = 16; indices[28] = 18; indices[29] = 19;

	// Fill in the right face index data
	indices[30] = 20; indices[31] = 21; indices[32] = 22;
	indices[33] = 20; indices[34] = 22; indices[35] = 23;

	//calcnormal
	for (int i = 0; i < indices.size(); )
	{
		int index1 = indices[i];
		int index2 = indices[++i];
		int index3 = indices[++i];

		acm::float3 normal = CalcNoraml(vertices[index1].Pos, vertices[index2].Pos, vertices[index3].Pos);

		vertices[index1].Normal.x += normal.x; vertices[index1].Normal.y += normal.y; vertices[index1].Normal.z += normal.z;
		vertices[index2].Normal.x += normal.x; vertices[index2].Normal.y += normal.y; vertices[index2].Normal.z += normal.z;
		vertices[index3].Normal.x += normal.x; vertices[index3].Normal.y += normal.y; vertices[index3].Normal.z += normal.z;

		++i;
	}

	for (int i = 0; i < vertices.size(); ++i)
	{
		XMVECTOR vecNormal = XMVector3Normalize(XMLoadFloat3(&vertices[i].Normal));
		XMStoreFloat3(&vertices[i].Normal, vecNormal);
	}


	//create buffer vertex;
	meshData.vecVertex = vertices;

	D3D11_BUFFER_DESC vBufferDesc;
	vBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vBufferDesc.ByteWidth = meshData.vertexSize * (UINT)vertices.size();
	vBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vBufferDesc.CPUAccessFlags = 0;
	vBufferDesc.MiscFlags = 0;
	vBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vInitData;
	vInitData.pSysMem = &meshData.vecVertex[0];

	HR(RenderDevice::This().GetDevice()->CreateBuffer(&vBufferDesc, &vInitData, &meshData.pMeshBuffer.vBuffer), L"CreateBuffer Vertex Mesh Buffer Fail");

	//create buffer indices;
	meshData.vecIndics = indices;

	D3D11_BUFFER_DESC vIdxBufferDesc;
	vIdxBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vIdxBufferDesc.ByteWidth = sizeof(UINT) * (UINT)indices.size();
	vIdxBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	vIdxBufferDesc.CPUAccessFlags = 0;
	vIdxBufferDesc.MiscFlags = 0;
	vIdxBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA iIdxInitData;
	iIdxInitData.pSysMem = &meshData.vecIndics[0];

	HR(RenderDevice::This().GetDevice()->CreateBuffer(&vIdxBufferDesc, &iIdxInitData, &meshData.pMeshBuffer.vIBuffer), L"CreateBuffer Index Mesh Buffer Fail");

	return true;
}

bool MeshFactory::CreateBillboard(Billboard & _billboardData, std::vector<Vertex_Billboard>& _data)
{
	//정점 버퍼 생성

	_billboardData.vertexSize = sizeof(Vertex_Billboard);

	//create buffer vertex;
	_billboardData.vecVertex = _data;

	D3D11_BUFFER_DESC vBufferDesc;
	vBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vBufferDesc.ByteWidth = _billboardData.vertexSize * (UINT)_data.size();
	vBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vBufferDesc.CPUAccessFlags = 0;
	vBufferDesc.MiscFlags = 0;
	vBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vInitData;
	vInitData.pSysMem = &_billboardData.vecVertex[0];

	if (FAILED(RenderDevice::This().GetDevice()->CreateBuffer(&vBufferDesc, &vInitData, &_billboardData.vBuffer)))
	{
		return false;
	}

	return true;
}

bool MeshFactory::CreateInstance(Mesh& meshData, Instancing & pInstance)
{
	meshData.Init();

	D3D11_BUFFER_DESC InsBuffer;
	InsBuffer.Usage = D3D11_USAGE_DYNAMIC;
	InsBuffer.ByteWidth = sizeof(Vertex_Instance) * 100;
	InsBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	InsBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	InsBuffer.MiscFlags = 0;
	InsBuffer.StructureByteStride = 0;

	//RenderDevice::This().GetDevice()->CreateBuffer(&InsBuffer, 0, )

	return false;
}

bool MeshFactory::CreateGrid(int x, int z, float depth, Mesh & meshData)
{
	meshData.Init();
	meshData.vertexSize = sizeof(Vertex_Tex);

	UINT vertexCount = x * z;
	UINT IndiceCount = (x - 1) * (z - 1) * 6;

	meshData.vecVertex.resize(vertexCount);
	meshData.vecIndics.resize(IndiceCount);

	//정점 생성
	for (int i = 0; i < z; ++i)
	{
		for (int j = 0; j < x; ++j)
		{
			meshData.vecVertex[(i * x) + j].Pos = acm::float3((float)j, 0.0f, (float)i);
			meshData.vecVertex[(i * x) + j].Normal = acm::float3(0,0,0);
			meshData.vecVertex[(i * x) + j].Tangent = acm::float3(0, 0, 0);
			meshData.vecVertex[(i * x) + j].Tex = acm::float2((float)j / (float)x, 1.f - ((float)i / (float)z));
		}
	}

	//인덱스 생성
	UINT k = -1;
	for (int i = 0; i < z - 1; ++i)
	{
		for (int j = 0; j < x - 1; ++j)
		{
			meshData.vecIndics[++k] = (i * x) + j;
			meshData.vecIndics[++k] = ((i + 1) * x) + j;
			meshData.vecIndics[++k] = ((i + 1) * x) + j + 1;

			meshData.vecIndics[++k] = (i * x) + j;
			meshData.vecIndics[++k] = ((i + 1) * x) + j + 1;
			meshData.vecIndics[++k] = (i * x) + j + 1;
		}
	}

	if(meshData.vecIndics.size() != (++k))
	{
		//인덱스가 맞지않아여
		return false;
	}

	// 정점 버퍼 생성
	D3D11_BUFFER_DESC vBufferDesc;
	vBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vBufferDesc.ByteWidth = meshData.vertexSize * vertexCount;
	vBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vBufferDesc.CPUAccessFlags = 0;
	vBufferDesc.MiscFlags = 0;
	vBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vInitData;
	vInitData.pSysMem = &(meshData.vecVertex[0]);

	if (FAILED(RenderDevice::This().GetDevice()->CreateBuffer(&vBufferDesc, &vInitData, &meshData.pMeshBuffer.vBuffer)))
	{
		return false;
	}

	// 인덱스 버퍼 생성
	D3D11_BUFFER_DESC vIdxBufferDesc;
	vIdxBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vIdxBufferDesc.ByteWidth = sizeof(UINT) * IndiceCount;
	vIdxBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	vIdxBufferDesc.CPUAccessFlags = 0;
	vIdxBufferDesc.MiscFlags = 0;
	vIdxBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA iIdxInitData;
	iIdxInitData.pSysMem = &(meshData.vecIndics[0]);

	if (FAILED(RenderDevice::This().GetDevice()->CreateBuffer(&vIdxBufferDesc, &iIdxInitData, &meshData.pMeshBuffer.vIBuffer)))
	{
		return false;
	}

	return true;
}

bool MeshFactory::CreateSphere(float radius, UINT numSubdivisions, Mesh& meshData)
{
	meshData.vecVertex.clear();
	meshData.vecIndics.clear();

	//
	// Compute the vertices stating at the top pole and moving down the stacks.
	//

	// Poles: note that there will be texture coordinate distortion as there is
	// not a unique point on the texture map to assign to the pole when mapping
	// a rectangular texture onto a sphere.
	Vertex_Tex topVertex;
	topVertex.Pos = acm::float3(0.0f, +radius, 0.0f);
	topVertex.Normal = acm::float3(0.0f, 1.0f, 0.0f);
	topVertex.Tangent = acm::float3(1.0f, 0.0f, 0.0f);
	topVertex.Tex = acm::float2(0.0f, 0.0f);

	Vertex_Tex bottomVertex;
	bottomVertex.Pos = acm::float3(0.0f, -radius, 0.0f);
	bottomVertex.Normal = acm::float3(0.0f, -1.0f, 0.0f);
	bottomVertex.Tangent = acm::float3(1.0f, 0.0f, 0.0f);
	bottomVertex.Tex = acm::float2(0.0f, 1.0f);

	meshData.vecVertex.push_back(topVertex);

	float phiStep = XM_PI / numSubdivisions;
	float thetaStep = 2.0f*XM_PI / numSubdivisions;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for (UINT i = 1; i <= numSubdivisions - 1; ++i)
	{
		float phi = i*phiStep;

		// Vertices of ring.
		for (UINT j = 0; j <= numSubdivisions; ++j)
		{
			float theta = j*thetaStep;

			Vertex_Tex v;

			// spherical to cartesian
			v.Pos.x = radius*sinf(phi)*cosf(theta);
			v.Pos.y = radius*cosf(phi);
			v.Pos.z = radius*sinf(phi)*sinf(theta);

			// Partial derivative of P with respect to theta
			v.Tangent.x = -radius*sinf(phi)*sinf(theta);
			v.Tangent.y = 0.0f;
			v.Tangent.z = +radius*sinf(phi)*cosf(theta);

			XMVECTOR T = XMLoadFloat3(&v.Tangent);
			XMStoreFloat3(&v.Tangent, XMVector3Normalize(T));

			XMVECTOR p = XMLoadFloat3(&v.Pos);
			XMStoreFloat3(&v.Normal, XMVector3Normalize(p));

			XMStoreFloat3(&v.BiNormal, XMVector3Cross(XMLoadFloat3(&v.Normal), XMLoadFloat3(&v.Tangent)));

			v.Tex.x = theta / XM_2PI;
			v.Tex.y = phi / XM_PI;

			meshData.vecVertex.push_back(v);
		}
	}

	meshData.vecVertex.push_back(bottomVertex);

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//

	for (UINT i = 1; i <= numSubdivisions; ++i)
	{
		meshData.vecIndics.push_back(0);
		meshData.vecIndics.push_back(i + 1);
		meshData.vecIndics.push_back(i);
	}

	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	UINT baseIndex = 1;
	UINT ringVertexCount = numSubdivisions + 1;
	for (UINT i = 0; i < numSubdivisions - 2; ++i)
	{
		for (UINT j = 0; j < numSubdivisions; ++j)
		{
			meshData.vecIndics.push_back(baseIndex + i*ringVertexCount + j);
			meshData.vecIndics.push_back(baseIndex + i*ringVertexCount + j + 1);
			meshData.vecIndics.push_back(baseIndex + (i + 1)*ringVertexCount + j);

			meshData.vecIndics.push_back(baseIndex + (i + 1)*ringVertexCount + j);
			meshData.vecIndics.push_back(baseIndex + i*ringVertexCount + j + 1);
			meshData.vecIndics.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	UINT southPoleIndex = (UINT)meshData.vecVertex.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (UINT i = 0; i < numSubdivisions; ++i)
	{
		meshData.vecIndics.push_back(southPoleIndex);
		meshData.vecIndics.push_back(baseIndex + i);
		meshData.vecIndics.push_back(baseIndex + i + 1);
	}

	return true;
}

bool MeshFactory::CreateGeoSphere(float radius, UINT numSubdivisions, Mesh& meshData)
{
	meshData.Init();
	const float x = 0.525731f;
	const float z = 0.850651f;

	meshData.vertexSize = sizeof(Vertex_Tex);

	std::vector<Vertex_Tex> vertices =
	{
		{ acm::float3(-x, 0.0f, z) },
		{ acm::float3(x, 0.0f, z)  },
		{ acm::float3(-x, 0.0f, -z) },
		{ acm::float3(x, 0.0f, -z) },

		{ acm::float3(0.0f, z, x)  },
		{ acm::float3(0.0f, z, -x) },
		{ acm::float3(0.0f, -z, x) },
		{ acm::float3(0.0f, -z, -x) },

		{ acm::float3(z, x, 0.0f) },
		{ acm::float3(-z, x, 0.0f) },
		{ acm::float3(z, -x, 0.0f) },
		{ acm::float3(-z, -x, 0.0f) },
	};

	meshData.vecVertex.assign(vertices.begin(), vertices.end());

	std::vector<UINT> indices = {
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
	};

	meshData.vecIndics.assign(indices.begin(), indices.end());

	Subdivide(meshData, numSubdivisions);

	// Project vertices onto sphere and scale.
	for (UINT i = 0; i < meshData.vecVertex.size(); ++i)
	{
		// Project onto unit sphere.
		XMVECTOR n = XMVector3Normalize(XMLoadFloat3(&meshData.vecVertex[i].Pos));

		// Project onto sphere.
		XMVECTOR p = radius * n;

		XMStoreFloat3(&meshData.vecVertex[i].Pos, p);
		XMStoreFloat3(&meshData.vecVertex[i].Normal, n);

		// Derive texture coordinates from spherical coordinates.
		float theta = acm::AngleFromXY(
			meshData.vecVertex[i].Pos.x,
			meshData.vecVertex[i].Pos.z);

		float phi = acosf(meshData.vecVertex[i].Pos.y / radius);

		meshData.vecVertex[i].Tex.x = theta / XM_2PI;
		meshData.vecVertex[i].Tex.y = phi / XM_PI;

		// Partial derivative of P with respect to theta
		meshData.vecVertex[i].Tangent.x = -radius*sinf(phi)*sinf(theta);
		meshData.vecVertex[i].Tangent.y = 0.0f;
		meshData.vecVertex[i].Tangent.z = +radius*sinf(phi)*cosf(theta);

		XMVECTOR T = XMLoadFloat3(&meshData.vecVertex[i].Tangent);
		XMStoreFloat3(&meshData.vecVertex[i].Tangent, XMVector3Normalize(T));

		XMStoreFloat3(&meshData.vecVertex[i].BiNormal, XMVector3Cross(XMLoadFloat3(&meshData.vecVertex[i].Normal), XMLoadFloat3(&meshData.vecVertex[i].Tangent)));
	}

	// 정점 버퍼 생성
	D3D11_BUFFER_DESC vBufferDesc;
	vBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vBufferDesc.ByteWidth = meshData.vertexSize * (UINT)meshData.vecVertex.size();
	vBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vBufferDesc.CPUAccessFlags = 0;
	vBufferDesc.MiscFlags = 0;
	vBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vInitData;
	vInitData.pSysMem = &(meshData.vecVertex[0]);

	if (FAILED(RenderDevice::This().GetDevice()->CreateBuffer(&vBufferDesc, &vInitData, &meshData.pMeshBuffer.vBuffer)))
	{
		return false;
	}

	// 인덱스 버퍼 생성
	D3D11_BUFFER_DESC vIdxBufferDesc;
	vIdxBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vIdxBufferDesc.ByteWidth = sizeof(UINT) * (UINT)meshData.vecIndics.size();
	vIdxBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	vIdxBufferDesc.CPUAccessFlags = 0;
	vIdxBufferDesc.MiscFlags = 0;
	vIdxBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA iIdxInitData;
	iIdxInitData.pSysMem = &(meshData.vecIndics[0]);

	if (FAILED(RenderDevice::This().GetDevice()->CreateBuffer(&vIdxBufferDesc, &iIdxInitData, &meshData.pMeshBuffer.vIBuffer)))
	{
		return false;
	}

	return true;
}

void MeshFactory::Subdivide(Mesh& meshData, UINT numSubdivisions)
{
	if( 0 == numSubdivisions)
	{
		return;
	}
	// Save a copy of the input geometry.
	Mesh inputCopy = meshData;

	meshData.vecVertex.resize(0);
	meshData.vecIndics.resize(0);

//       v1
//       *
//      / \
//     /   \
//  m0*-----*m1
//   / \   / \
//  /   \ /   \
// *-----*-----*
// v0    m2     v2

	UINT numTris = (UINT)inputCopy.vecIndics.size() / 3;
	for (UINT i = 0; i < numTris; ++i)
	{
		Vertex_Tex v0 = inputCopy.vecVertex[inputCopy.vecIndics[i * 3 + 0]];
		Vertex_Tex v1 = inputCopy.vecVertex[inputCopy.vecIndics[i * 3 + 1]];
		Vertex_Tex v2 = inputCopy.vecVertex[inputCopy.vecIndics[i * 3 + 2]];

		//
		// Generate the midpoints.
		//

		Vertex_Tex m0, m1, m2;

		// For subdivision, we just care about the position component.  We derive the other
		// vertex components in CreateGeosphere.

		m0.Pos = acm::float3(
			0.5f*(v0.Pos.x + v1.Pos.x),
			0.5f*(v0.Pos.y + v1.Pos.y),
			0.5f*(v0.Pos.z + v1.Pos.z));


		m1.Pos = acm::float3(
			0.5f*(v1.Pos.x + v2.Pos.x),
			0.5f*(v1.Pos.y + v2.Pos.y),
			0.5f*(v1.Pos.z + v2.Pos.z));


		m2.Pos = acm::float3(
			0.5f*(v0.Pos.x + v2.Pos.x),
			0.5f*(v0.Pos.y + v2.Pos.y),
			0.5f*(v0.Pos.z + v2.Pos.z));

		//
		// Add new geometry.
		//

		meshData.vecVertex.push_back(v0); // 0
		meshData.vecVertex.push_back(v1); // 1
		meshData.vecVertex.push_back(v2); // 2
		meshData.vecVertex.push_back(m0); // 3
		meshData.vecVertex.push_back(m1); // 4
		meshData.vecVertex.push_back(m2); // 5

		meshData.vecIndics.push_back(i * 6 + 0);
		meshData.vecIndics.push_back(i * 6 + 3);
		meshData.vecIndics.push_back(i * 6 + 5);

		meshData.vecIndics.push_back(i * 6 + 3);
		meshData.vecIndics.push_back(i * 6 + 4);
		meshData.vecIndics.push_back(i * 6 + 5);

		meshData.vecIndics.push_back(i * 6 + 5);
		meshData.vecIndics.push_back(i * 6 + 4);
		meshData.vecIndics.push_back(i * 6 + 2);

		meshData.vecIndics.push_back(i * 6 + 3);
		meshData.vecIndics.push_back(i * 6 + 1);
		meshData.vecIndics.push_back(i * 6 + 4);
	}

	Subdivide(meshData, --numSubdivisions);
}

acm::float3 MeshFactory::CalcNoraml(acm::float3 v0, acm::float3 v1, acm::float3 v2)
{
	XMVECTOR u = XMLoadFloat3(&v1) - XMLoadFloat3(&v0);
	XMVECTOR v = XMLoadFloat3(&v2) - XMLoadFloat3(&v0);

	acm::float3 normal;
	XMStoreFloat3(&normal, XMVector3Normalize(XMVector3Cross(u, v)));

	return normal;
}

bool MeshFactory::CreateFBXMesh(std::string FilePath, Mesh& meshData)
{
	FbxScene* pScene = FBXUtill::This().ImportFBXScene(FilePath.c_str());

	meshData.meshFilename = FilePath;
	meshData.vertexSize = sizeof(Vertex_Tex);
	ConvertFBXMesh(&meshData, pScene->GetRootNode());

	pScene->Destroy();

	return CreateBuffer(meshData);
}

void MeshFactory::ConvertFBXMesh(Mesh* meshData, FbxNode* fbNode)
{
	meshData->Init();
	FbxMesh * currMesh = fbNode->GetMesh();

	std::vector<std::string> vecTextureName;

	int materialCount = fbNode->GetMaterialCount();
	for (int i = 0; i < materialCount; ++i)
	{
		char temp[MAX_PATH] = { 0 };
		sprintf_s(temp, MAX_PATH, "%s%s", FileSystem::GetFilePath(meshData->meshFilename).c_str(), fbNode->GetMaterial(i)->GetName());
		vecTextureName.push_back(std::string(FileSystem::SwitchFileFormat(temp, ".dds")));
	}

	if (materialCount > 0)
	{
		meshData->pTexture[Mesh::TEXTURE_TYPE_DIFFUSE] = TextureManager::This().LoadTexture2DArray(meshData->meshFilename, vecTextureName);
	}

	if (currMesh != nullptr)
	{
		//vertex
		size_t vertexSize = currMesh->GetControlPointsCount();
		meshData->vecVertex.resize(vertexSize);
		for (size_t i = 0; i < vertexSize; ++i)
		{
			meshData->vecVertex[i].Pos.x = static_cast<float>(currMesh->GetControlPointAt((int)i).mData[0]);
			meshData->vecVertex[i].Pos.y = static_cast<float>(currMesh->GetControlPointAt((int)i).mData[1]);
			meshData->vecVertex[i].Pos.z = static_cast<float>(currMesh->GetControlPointAt((int)i).mData[2]);
		}


		//Indices
		size_t IdxCount = currMesh->GetPolygonVertexCount();
		int* pIdx = currMesh->GetPolygonVertices();

		//Normal
		FbxLayerElementArrayTemplate<FbxVector4>* pNormal;
		currMesh->GetNormals(&pNormal);

		//UV
		FBXUtill::This().ReadFBXUVData(currMesh, meshData);

		//TexID
		FBXUtill::This().ReadFBXMaterialData(currMesh, meshData);

		meshData->vecIndics.resize(IdxCount);
		for (size_t i = 0; i < IdxCount; ++i)
		{
			meshData->vecIndics[i] = pIdx[i];

			meshData->vecVertex[pIdx[i]].Normal.x = static_cast<float>(pNormal->GetAt(pIdx[i]).mData[0]);
			meshData->vecVertex[pIdx[i]].Normal.y = static_cast<float>(pNormal->GetAt(pIdx[i]).mData[1]);
			meshData->vecVertex[pIdx[i]].Normal.z = static_cast<float>(pNormal->GetAt(pIdx[i]).mData[2]);
		}
	}

	int childcount = fbNode->GetChildCount();
	meshData->vecChildMesh.resize(childcount);

	for (int i = 0; i < childcount; ++i)
	{
		meshData->vecChildMesh[i] = new Mesh();
		meshData->vecChildMesh[i]->vertexSize = meshData->vertexSize;
		meshData->vecChildMesh[i]->meshFilename = meshData->meshFilename;
		ConvertFBXMesh(meshData->vecChildMesh[i], fbNode->GetChild(i));
	}
}

bool MeshFactory::CreateBuffer(Mesh & meshData)
{
	if (meshData.GetVertexCount() > 0 && meshData.GetIndicsCount() > 0)
	{
		//create buffer vertex;
		D3D11_BUFFER_DESC vBufferDesc;
		vBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vBufferDesc.ByteWidth = meshData.vertexSize * (UINT)meshData.GetVertexCount();
		vBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vBufferDesc.CPUAccessFlags = 0;
		vBufferDesc.MiscFlags = 0;
		vBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vInitData;
		vInitData.pSysMem = &meshData.vecVertex[0];

		HR(RenderDevice::This().GetDevice()->CreateBuffer(&vBufferDesc, &vInitData, &meshData.pMeshBuffer.vBuffer), L"CreateBuffer Vertex Mesh Buffer Fail");

		//create buffer indices;
		D3D11_BUFFER_DESC vIdxBufferDesc;
		vIdxBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vIdxBufferDesc.ByteWidth = sizeof(UINT) * (UINT)meshData.GetIndicsCount();
		vIdxBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		vIdxBufferDesc.CPUAccessFlags = 0;
		vIdxBufferDesc.MiscFlags = 0;
		vIdxBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA iIdxInitData;
		iIdxInitData.pSysMem = &meshData.vecIndics[0];

		HR(RenderDevice::This().GetDevice()->CreateBuffer(&vIdxBufferDesc, &iIdxInitData, &meshData.pMeshBuffer.vIBuffer), L"CreateBuffer Index Mesh Buffer Fail");
	}

	for (auto iter = meshData.vecChildMesh.begin(); iter != meshData.vecChildMesh.end(); ++iter)
	{
		Mesh* pMesh = (*iter);
		CreateBuffer(*pMesh);
	}

	return true;
}
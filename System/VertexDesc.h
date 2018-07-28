#pragma once
#include "DirectXMath.h"
#include "d3d11.h"
#include <unordered_map>
#include "../CommonUtil/Singletone.h"
//#include "../CommonUtil/StringUtil.h"

using namespace DirectX;

struct Vertex_RenderTarget
{
	acm::float3 Pos;
	acm::float2 UV;
};

static D3D11_INPUT_ELEMENT_DESC Vertex_RenderTarget_Desc[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

struct Vertex_Color
{
	acm::float3 Pos;
	acm::float4 Color;
};

static D3D11_INPUT_ELEMENT_DESC Vertex_Color_Desc[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

struct Vertex_Debug
{
	acm::float3 Pos;
};

static D3D11_INPUT_ELEMENT_DESC Vertex_Debug_Desc[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

struct Vertex_Tex
{
	acm::float3 Pos;
	acm::float3 Normal;
	acm::float3 Tangent;
	acm::float3 BiNormal;
	acm::float2 Tex;
	UINT	 TexID;
};

static D3D11_INPUT_ELEMENT_DESC Vertex_Tex_Desc[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",	  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BINORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXID",    0, DXGI_FORMAT_R32_UINT, 0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

struct Vertex_Billboard
{
	acm::float3 Pos;
	acm::float2 Size;
};

static D3D11_INPUT_ELEMENT_DESC Vertex_Billboard_Desc[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "SIZE",	  0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

struct Vertex_Instance
{
	acm::float4x4 World;
};

static D3D11_INPUT_ELEMENT_DESC Vertex_Instance_Desc[] =
{
	{ "WORLD",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "WORLD",	  1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "WORLD",	  2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "WORLD",	  3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
};

//struct Vertex_VoxelTerrain_Instance
//{
//	acm::float4	Position;
//};

struct Vertex_Voxel_Terrain
{
	acm::float3 _voxelInfo;
};

static D3D11_INPUT_ELEMENT_DESC Vertex_Voxel_Terrain_Desc[] =
{
	{ "VOXELINFO",	  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

struct Vertex_VoxelTerrain_Instance
{
	acm::float3	PosIndex;
	acm::float4	Density0;
	acm::float4	Density1;
	acm::float3	Normal0;
	acm::float3	Normal1;
	acm::float3	Normal2;
	acm::float3	Normal3;
	acm::float3	Normal4;
	acm::float3	Normal5;
	acm::float3	Normal6;
	acm::float3	Normal7;
};

static D3D11_INPUT_ELEMENT_DESC Vertex_VoxelTerrain_Instance_Desc[] =
{
	{ "POSITION",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "DENSITY",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "DENSITY",	  1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "FNORMAL",	  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "FNORMAL",	  1, DXGI_FORMAT_R32G32B32_FLOAT, 0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "FNORMAL",	  2, DXGI_FORMAT_R32G32B32_FLOAT, 0, 68, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "FNORMAL",	  3, DXGI_FORMAT_R32G32B32_FLOAT, 0, 80, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BNORMAL",	  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 92, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BNORMAL",	  1, DXGI_FORMAT_R32G32B32_FLOAT, 0, 92, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BNORMAL",	  2, DXGI_FORMAT_R32G32B32_FLOAT, 0, 104, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BNORMAL",	  3, DXGI_FORMAT_R32G32B32_FLOAT, 0, 116, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

struct TerrainVertex
{
	acm::float3 Pos;
	acm::float3 Normal;
	acm::float3 Tangent;
	acm::float2 UV;
};

struct VertexDescInfo
{
	int							ElementDescSize;
	D3D11_INPUT_ELEMENT_DESC*   ElementDesc;

	VertexDescInfo(int _elementSize, D3D11_INPUT_ELEMENT_DESC* _elementDesc)
	{
		ElementDescSize = _elementSize; ElementDesc = _elementDesc;
	}
};

class VertexDesc : public Singletone<VertexDesc>
{
public:
	VertexDesc() {
		// 이것도 어찌 옮겨야할텐디
		m_vDescMap.insert(std::pair<std::string, VertexDescInfo>("PackGBufferTech", VertexDescInfo(sizeof(Vertex_Tex_Desc), Vertex_Tex_Desc)));
		m_vDescMap.insert(std::pair<std::string, VertexDescInfo>("LightDefaultTech", VertexDescInfo(sizeof(Vertex_Tex_Desc), Vertex_Tex_Desc)));
		m_vDescMap.insert(std::pair<std::string, VertexDescInfo>("LightBillboardTech", VertexDescInfo(sizeof(Vertex_Billboard_Desc), Vertex_Billboard_Desc)));
		m_vDescMap.insert(std::pair<std::string, VertexDescInfo>("ScreenSquadTech", VertexDescInfo(sizeof(Vertex_RenderTarget_Desc), Vertex_RenderTarget_Desc)));
		m_vDescMap.insert(std::pair<std::string, VertexDescInfo>("SkyTech", VertexDescInfo(sizeof(Vertex_Tex_Desc), Vertex_Tex_Desc)));
		m_vDescMap.insert(std::pair<std::string, VertexDescInfo>("HorizonBlur", VertexDescInfo(0, nullptr)));
		m_vDescMap.insert(std::pair<std::string, VertexDescInfo>("VerticalBlur", VertexDescInfo(0, nullptr)));
		m_vDescMap.insert(std::pair<std::string, VertexDescInfo>("PerlinNoise", VertexDescInfo(sizeof(Vertex_RenderTarget_Desc), Vertex_RenderTarget_Desc)));
		m_vDescMap.insert(std::pair<std::string, VertexDescInfo>("TerrainTech", VertexDescInfo(sizeof(Vertex_VoxelTerrain_Instance_Desc), Vertex_VoxelTerrain_Instance_Desc)));
		m_vDescMap.insert(std::pair<std::string, VertexDescInfo>("TerrainVoxelTech", VertexDescInfo(sizeof(Vertex_Voxel_Terrain_Desc), Vertex_Voxel_Terrain_Desc)));
		m_vDescMap.insert(std::pair<std::string, VertexDescInfo>("DebugTech", VertexDescInfo(sizeof(Vertex_Debug_Desc), Vertex_Debug_Desc)));
	};
	~VertexDesc() {};

private:
	std::unordered_map<std::string, VertexDescInfo> m_vDescMap;

public:
	int GetElementSize(const std::string& FXName)
	{
		auto iter = m_vDescMap.find(FXName);
		if (m_vDescMap.end() != iter)
		{
			return iter->second.ElementDescSize;
		}

		return 0;
	};

	D3D11_INPUT_ELEMENT_DESC* GetElementDesc(const std::string& FXName)
	{
		auto iter = m_vDescMap.find(FXName);
		if (m_vDescMap.end() != iter)
		{
			return iter->second.ElementDesc;
		}

		return nullptr;
	};
};
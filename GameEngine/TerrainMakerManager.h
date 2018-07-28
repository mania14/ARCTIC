#pragma once
#include "../CommonUtil/Singletone.h"
#include "DirectXMath.h"

class Terrain;
struct MeshBuffer;
class TerrainMakerManager : public Singletone<TerrainMakerManager>
{
public:
	explicit TerrainMakerManager() {};
	virtual ~TerrainMakerManager() {};

public:
	void	MakeHeightMap(int x, int y, std::string filename);
	void	Init();
	void	Render();
	void	Release();

private:
	std::vector<acm::float3>					_TerrainVoxelVertexLookUpTable;
	std::vector<std::vector<UINT>>					_TerrainVoxelIdxLookUpTable;
	std::vector<byte>								_TerrainVoxelMaskLookUpTable;
	std::vector<MeshBuffer>							_TerrainCellMeshLookUpBuffer;


	Terrain*										pTerrain;
};
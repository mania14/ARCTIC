#include "Component.h"
#include <vector>

struct ID3D11Buffer;
struct Vertex_VoxelTerrain_Instance;
class Terrain : public Component
{
public:
	const static COMPONENTIDTYPE COMPONENTID = TERRAIN_COMPONENT;

public:
	explicit Terrain();
	virtual ~Terrain();

public:
	virtual int				Init();
	virtual int				Release();
	int						Update();
	void					TestRender();
	COMPONENTIDTYPE GetComponentID() override {
		return COMPONENTID;
	};

private:
	std::vector<float>							_TerrainVoxelDensity[33][33][33];
	std::vector<Vertex_VoxelTerrain_Instance*>	_vecTerrainVoxel;
	ID3D11Buffer*								_vInstanceBuffer;
	ID3D11Buffer*								_vTempBuffer;
};
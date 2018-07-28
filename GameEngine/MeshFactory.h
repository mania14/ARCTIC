#pragma once
#include "../CommonUtil/Singletone.h"
#include "FBXUtill.h"
#include "../System/VertexDesc.h"

class Mesh;
class Billboard;
class Instancing;

class MeshFactory : public Singletone<MeshFactory>
{
public:
	MeshFactory();
	~MeshFactory();

public:
	void Init();
	void Release();
	bool CreateGrid(int x, int z, float depth, Mesh& meshData);
	bool CreateTerrainVoxel(MeshBuffer& meshBuffer, std::vector<Vertex_Voxel_Terrain>& _data, std::vector<UINT>& _index);
	bool CreateBox(Mesh& meshData);
	bool CreateSphere(float radius, UINT numSubdivisions, Mesh& meshData);
	bool CreateGeoSphere(float radius, UINT numSubdivisions, Mesh& meshData);
	bool CreateBillboard(Billboard& _billboardData, std::vector<Vertex_Billboard>& _data);
	bool CreateInstance(Mesh& meshData, Instancing& pInstance);
	bool CreateFBXMesh(std::string FilePath, Mesh& meshData);
private:
	void Subdivide(Mesh& meshData, UINT numSubdivisions);
	acm::float3 CalcNoraml(acm::float3 v0, acm::float3 v1, acm::float3 v2);
	bool CreateBuffer(Mesh& meshData);
	void ConvertFBXMesh(Mesh* meshData, FbxNode* fbNode);
}; 
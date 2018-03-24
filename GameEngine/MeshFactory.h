#pragma once
#include "../CommonUtil/Singletone.h"
#include "FBXUtill.h"

class Mesh;
class Billboard;
class Instancing;
struct Vertex_Billboard;

class MeshFactory : public Singletone<MeshFactory>
{
public:
	MeshFactory();
	~MeshFactory();

public:
	void Init();
	void Release();
	bool CreateGrid(int x, int z, float depth, Mesh& meshData);
	bool CreateBox(Mesh& meshData);
	bool CreateSphere(float radius, UINT numSubdivisions, Mesh& meshData);
	bool CreateGeoSphere(float radius, UINT numSubdivisions, Mesh& meshData);
	bool CreateBillboard(Billboard& _billboardData, std::vector<Vertex_Billboard>& _data);
	bool CreateInstance(Mesh& meshData, Instancing& pInstance);
	bool CreateFBXMesh(std::string FilePath, Mesh& meshData);
private:
	void Subdivide(Mesh& meshData, UINT numSubdivisions);
	XMFLOAT3 CalcNoraml(XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2);
	bool CreateBuffer(Mesh& meshData);
	void ConvertFBXMesh(Mesh* meshData, FbxNode* fbNode);
}; 
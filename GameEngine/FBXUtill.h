#pragma once
#include "../CommonUtil/Singletone.h"
#include <fbxsdk.h>

class Mesh;
class FBXUtill : public Singletone<FBXUtill>
{
public:
	FBXUtill::FBXUtill();
	FBXUtill::~FBXUtill();

public:
	bool Init();
	bool Release();
	FbxScene* ImportFBXScene(const char* FileName);
	void      ReadFBXUVData(FbxMesh* pFbxMesh, Mesh* pMesh);
	void      ReadFBXNormalData(FbxGeometryElementNormal* pElement, XMFLOAT3* pData);
	void      ReadFBXMaterialData(FbxMesh* pFbxMesh, Mesh* pMesh);

private:
	FbxManager* pManager = NULL;
	FbxScene* pScene = NULL;
};
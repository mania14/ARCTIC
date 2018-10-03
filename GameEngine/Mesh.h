#include "Component.h"
#include "../System/VertexDesc.h"
#include "../System/RenderDevice.h"
#include "../System/Texture.h"
#include <vector>

struct MeshBuffer;
struct Material;

class Mesh : public Component
{
public:
	const static COMPONENTIDTYPE COMPONENTID = MESH_COMPONENT;

public:
	enum TextureType {
		TEXTURE_SLOT0,
		TEXTURE_SLOT1,
		TEXTURE_SLOT2,
		TEXTURE_TYPE_COUNT
	};

public:
	explicit Mesh();
	virtual ~Mesh();

public:
	virtual int				Init();
	virtual int				Release();
	COMPONENTIDTYPE GetComponentID() override {
		return COMPONENTID;
	};

public:
	MeshBuffer* 			GetMeshBuffer() { return &pMeshBuffer; };
	const size_t			GetIndicsCount() { return vecIndics.size(); };
	const size_t			GetVertexCount() { return vecVertex.size(); };
	const UINT				GetVertexSize() { return vertexSize; };
	void					SetTexture(TextureType textureSlotNum, Texture* pTex);
	Texture*				GetTexture(TextureType textureSlotNum) {
		return pTexture[textureSlotNum];
	};
	void					Draw();
	
public:
	std::string				meshFilename;
	std::vector<Vertex_Tex>	vecVertex;
	std::vector<UINT>		vecIndics;
	UINT					vertexSize;
	MeshBuffer				pMeshBuffer;
	std::vector<Mesh*>		vecChildMesh;
	Texture*				pTexture[TEXTURE_TYPE_COUNT];
	Material*				pMaterial;
};
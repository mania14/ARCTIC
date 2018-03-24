#include "Component.h"
#include "../System/VertexDesc.h"
#include "../System/RenderDevice.h"
#include "Texture.h"
#include <vector>

struct MeshBuffer;
struct Material;

class Mesh : public Component
{
public:
	const static COMPONENTIDTYPE COMPONENTID = MESH_COMPONENT;

public:
	enum TextureType {
		TEXTURE_TYPE_DIFFUSE,
		TEXTURE_TYPE_NORMAL,
		TEXTURE_TYPE_EMMISIVE,
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
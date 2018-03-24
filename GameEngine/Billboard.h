#include "Component.h"
#include "../System/VertexDesc.h"
#include "../System/RenderDevice.h"
#include <vector>

struct BillboardBuffer;
class Billboard : public Component
{
public:
	const static COMPONENTIDTYPE COMPONENTID = BILLBOARD_COMPONENT;

public:
	explicit Billboard();
	virtual ~Billboard();

public:
	virtual int				Init();
	virtual int				Release();
	COMPONENTIDTYPE GetComponentID() override {
		return COMPONENTID;
	};

public:
	ID3D11Buffer* 			GetBillboardBuffer() { return vBuffer; };
	const size_t			GetVertexCount() { return vecVertex.size(); };
	const UINT				GetVertexSize() { return vertexSize; };

public:
	std::vector<Vertex_Billboard>	vecVertex;
	ID3D11Buffer*					vBuffer;
	UINT							vertexSize;
};
#include "Component.h"
#include "../System/VertexDesc.h"
#include "../System/RenderDevice.h"
#include <vector>

class Instancing : public Component
{
public:
	const static COMPONENTIDTYPE COMPONENTID = INSTANCING_COMPONENT;

public:
	explicit Instancing();
	virtual ~Instancing();

public:
	virtual int				Init();
	virtual int				Release();
	COMPONENTIDTYPE GetComponentID() override {
		return COMPONENTID;
	};

public:
	ID3D11Buffer* 			GetBuffer() { return vBuffer; };
	const size_t			GetVertexCount() { return vecVertex.size(); };
	const size_t			GetVertexSize() { return sizeof(Vertex_Instance); };

public:
	std::vector<Vertex_Instance>	vecVertex;
	ID3D11Buffer*					vBuffer;
};
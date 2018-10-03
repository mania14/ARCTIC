#pragma once
#include "../CommonUtil/Singletone.h"
#include <vector>
#include "../System/Texture.h"
#include "../System/VertexDesc.h"

struct MeshBuffer;
class RenderTargetManager : public Singletone<RenderTargetManager>
{
public:
	enum eRenderTarget : byte {
		eRT_BACKBUFFER,
		eRT_COLOR,
		eRT_NORMAL,
		eRT_SPECPOWER,
		eRT_BACKBUFFER_TEMP,
		eRT_BLUR,
		eRT_COUNT
	};

public:
	RenderTargetManager() {};
	~RenderTargetManager() {};

public:
	void Init();
	void Release();
	Texture* GetRenderTarget(eRenderTarget eRT) {
		return _RenderTargetList[eRT];
	};

	void DrawScreenQuad();
	void DrawScreenQuadRaw();

private:
	static const std::vector<Vertex_RenderTarget> _ScreenQuadVertices;
	static const std::vector<UINT>		 _ScreenQuadIndices;
	MeshBuffer*							 _ScreenQuadBuffer;

public:
	std::vector<Texture*>				_RenderTargetList;
};
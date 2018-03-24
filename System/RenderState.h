//***************************************************************************************
// RenderStates.h by Frank Luna (C) 2011 All Rights Reserved.
//   
// Defines render state objects.  
//***************************************************************************************
#pragma once
#include "../CommonUtil/Singletone.h"
#include <d3d11_1.h>

static float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

class RenderStates : public Singletone<RenderStates>
{
public:
	RenderStates() {};
	~RenderStates() {};

public:
	void InitAll(ID3D11Device* device);
	void Release();

	// Rasterizer states
	ID3D11RasterizerState* WireframeRS;
	ID3D11RasterizerState* NoCullRS;
	ID3D11RasterizerState* CullClockwiseRS;

	// Blend states
	ID3D11BlendState* AlphaToCoverageBS;
	ID3D11BlendState* TransparentBS;
	ID3D11BlendState* NoRenderTargetWritesBS;

	// Depth/stencil states
	ID3D11DepthStencilState* MarkMirrorDSS;
	ID3D11DepthStencilState* DrawReflectionDSS;
	ID3D11DepthStencilState* NoDoubleBlendDSS;
	ID3D11DepthStencilState* NoUseDepthStencilDSS;
	ID3D11DepthStencilState* ReadOnlyDepthStencilDSS;
};
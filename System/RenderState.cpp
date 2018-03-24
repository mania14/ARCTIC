//***************************************************************************************
// RenderStates.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "RenderState.h"
#include <d3d11_1.h>

void RenderStates::InitAll(ID3D11Device* device)
{
	{
		//
		// WireframeRS
		//
		D3D11_RASTERIZER_DESC wireframeDesc;
		ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
		wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
		wireframeDesc.CullMode = D3D11_CULL_NONE;
		wireframeDesc.FrontCounterClockwise = false;
		wireframeDesc.DepthClipEnable = true;

		device->CreateRasterizerState(&wireframeDesc, &WireframeRS);
	}

	{
		//
		// NoCullRS
		//
		D3D11_RASTERIZER_DESC noCullDesc;
		ZeroMemory(&noCullDesc, sizeof(D3D11_RASTERIZER_DESC));
		noCullDesc.FillMode = D3D11_FILL_SOLID;
		noCullDesc.CullMode = D3D11_CULL_NONE;
		noCullDesc.FrontCounterClockwise = false;
		noCullDesc.DepthClipEnable = true;

		device->CreateRasterizerState(&noCullDesc, &NoCullRS);
	}

	{
		//
		// CullClockwiseRS
		//

		// Note: Define such that we still cull backfaces by making front faces CCW.
		// If we did not cull backfaces, then we have to worry about the BackFace
		// property in the D3D11_DEPTH_STENCIL_DESC.
		D3D11_RASTERIZER_DESC cullClockwiseDesc;
		ZeroMemory(&cullClockwiseDesc, sizeof(D3D11_RASTERIZER_DESC));
		cullClockwiseDesc.FillMode = D3D11_FILL_SOLID;
		cullClockwiseDesc.CullMode = D3D11_CULL_BACK;
		cullClockwiseDesc.FrontCounterClockwise = true;
		cullClockwiseDesc.DepthClipEnable = true;

		device->CreateRasterizerState(&cullClockwiseDesc, &CullClockwiseRS);

		//
		// AlphaToCoverageBS
		//

		D3D11_BLEND_DESC alphaToCoverageDesc = { 0 };
		alphaToCoverageDesc.AlphaToCoverageEnable = true;
		alphaToCoverageDesc.IndependentBlendEnable = false;
		alphaToCoverageDesc.RenderTarget[0].BlendEnable = false;
		alphaToCoverageDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		device->CreateBlendState(&alphaToCoverageDesc, &AlphaToCoverageBS);
	}

	{
		//
		// TransparentBS
		//

		D3D11_BLEND_DESC transparentDesc = { 0 };
		//다중표본화
		transparentDesc.AlphaToCoverageEnable = false;
		//다중렌더
		transparentDesc.IndependentBlendEnable = false;

		transparentDesc.RenderTarget[0].BlendEnable = true;
		transparentDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		transparentDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		transparentDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		transparentDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		transparentDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		transparentDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		transparentDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		device->CreateBlendState(&transparentDesc, &TransparentBS);
	}

	{
		//
		// NoRenderTargetWritesBS
		//

		D3D11_BLEND_DESC noRenderTargetWritesDesc = { 0 };
		noRenderTargetWritesDesc.AlphaToCoverageEnable = false;
		noRenderTargetWritesDesc.IndependentBlendEnable = false;

		noRenderTargetWritesDesc.RenderTarget[0].BlendEnable = false;
		noRenderTargetWritesDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		noRenderTargetWritesDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		noRenderTargetWritesDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		noRenderTargetWritesDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		noRenderTargetWritesDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		noRenderTargetWritesDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		// 렌더타겟에 쓰지않겠다. 스텐실 버퍼에만 렌더링한다
		noRenderTargetWritesDesc.RenderTarget[0].RenderTargetWriteMask = 0;

		device->CreateBlendState(&noRenderTargetWritesDesc, &NoRenderTargetWritesBS);
	}

	{
		//
		// MarkMirrorDSS
		//

		D3D11_DEPTH_STENCIL_DESC mirrorDesc;
		// 깊이 판정 On
		mirrorDesc.DepthEnable = true;

		// 깊이버퍼도 안쓴다.
		mirrorDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		// 깊이버퍼를 쓸때의 통상적인 판정 (후면 버퍼에 기록되어있는 픽셀의 깊이보다 현재 깊이가 작은 경우 통과)
		mirrorDesc.DepthFunc = D3D11_COMPARISON_LESS;
		// 스텐실 판정 On
		mirrorDesc.StencilEnable = true;
		mirrorDesc.StencilReadMask = 0xff;
		mirrorDesc.StencilWriteMask = 0xff;

		// 스텐실 판정이 실패했을때
		mirrorDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		// 스텐실 판정은 성공, 깊이 판정은 실패!
		mirrorDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		// 스텐실 판정은 성공
		mirrorDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		//기본판정은 무조건 성공
		mirrorDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		//D3D11_STENCIL_OP_KEEP: 스텐실버퍼 변경X
		//D3D11_STENCIL_OP_ZERO: 스텐실버퍼 0으로 설정
		//D3D11_STENCIL_OP_REPLACE: 스텐실버퍼 ref설정값으로 덮어쓴다

		// We are not rendering backfacing polygons, so these settings do not matter.
		mirrorDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		mirrorDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		mirrorDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		mirrorDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		device->CreateDepthStencilState(&mirrorDesc, &MarkMirrorDSS);
	}

	{
		//
		// DrawReflectionDSS
		//

		D3D11_DEPTH_STENCIL_DESC drawReflectionDesc;
		drawReflectionDesc.DepthEnable = true;
		drawReflectionDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		drawReflectionDesc.DepthFunc = D3D11_COMPARISON_LESS;
		drawReflectionDesc.StencilEnable = true;
		drawReflectionDesc.StencilReadMask = 0xff;
		drawReflectionDesc.StencilWriteMask = 0xff;

		drawReflectionDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		drawReflectionDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		drawReflectionDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		drawReflectionDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

		// We are not rendering backfacing polygons, so these settings do not matter.
		drawReflectionDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		drawReflectionDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		drawReflectionDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		drawReflectionDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

		device->CreateDepthStencilState(&drawReflectionDesc, &DrawReflectionDSS);
	}

	{
		//
		// NoDoubleBlendDSS
		//
		D3D11_DEPTH_STENCIL_DESC noDoubleBlendDesc;
		noDoubleBlendDesc.DepthEnable = true;
		noDoubleBlendDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		noDoubleBlendDesc.DepthFunc = D3D11_COMPARISON_LESS;
		noDoubleBlendDesc.StencilEnable = true;
		noDoubleBlendDesc.StencilReadMask = 0xff;
		noDoubleBlendDesc.StencilWriteMask = 0xff;

		noDoubleBlendDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		noDoubleBlendDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		noDoubleBlendDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
		noDoubleBlendDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

		// We are not rendering backfacing polygons, so these settings do not matter.
		noDoubleBlendDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		noDoubleBlendDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		noDoubleBlendDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
		noDoubleBlendDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

		device->CreateDepthStencilState(&noDoubleBlendDesc, &NoDoubleBlendDSS);
	}

	{
		//
		// NoUseDepthStencil
		//
		D3D11_DEPTH_STENCIL_DESC noUseDepthStencilDesc;
		noUseDepthStencilDesc.DepthEnable = false;
		noUseDepthStencilDesc.StencilEnable = false;
		noUseDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		noUseDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		noUseDepthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		noUseDepthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		noUseDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		noUseDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		noUseDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
		noUseDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

		noUseDepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		noUseDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		noUseDepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
		noUseDepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

		device->CreateDepthStencilState(&noUseDepthStencilDesc, &NoUseDepthStencilDSS);
	}

	{
		//
		// NoWriteDepthStencil
		//
		D3D11_DEPTH_STENCIL_DESC ReadOnlyDepthStencilDesc;
		ReadOnlyDepthStencilDesc.DepthEnable = true;
		ReadOnlyDepthStencilDesc.StencilEnable = true;
		ReadOnlyDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		ReadOnlyDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		ReadOnlyDepthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		ReadOnlyDepthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		ReadOnlyDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		ReadOnlyDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		ReadOnlyDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		ReadOnlyDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

		ReadOnlyDepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		ReadOnlyDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		ReadOnlyDepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		ReadOnlyDepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

		device->CreateDepthStencilState(&ReadOnlyDepthStencilDesc, &ReadOnlyDepthStencilDSS);
	}

}

void RenderStates::Release()
{
	WireframeRS->Release();
	NoCullRS->Release();
	CullClockwiseRS->Release();

	AlphaToCoverageBS->Release();
	TransparentBS->Release();
	NoRenderTargetWritesBS->Release();

	MarkMirrorDSS->Release();
	DrawReflectionDSS->Release();
	NoDoubleBlendDSS->Release();
	NoUseDepthStencilDSS->Release();
}
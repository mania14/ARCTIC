#pragma once
#include "stdafx.h"
#include "../CommonUtil/Macro.h"
#include "../CommonUtil/Singletone.h"
#include "RenderState.h"
#include <d3d11_1.h>
#include <DirectXMath.h>
#include "d3dx11effect.h"
#include "FXManager.h"

//static const DirectX::XMVECTORF32 BackBufferColor = { 0.000000000f, 0.545098066f, 0.545098066f, 1.000000000f };
static const DirectX::XMVECTORF32 BackBufferColor = { 0.f, 0.f, 0.0f, 0.f };

class Texture;
enum eRENDER_STATE
{
	eRS_WIREFRAME,
	eRS_NO_CULL,
	eRS_CLOCKWISE,
	eRS_NO_USE_DEPTHSTENCIL,
	eRS_READONLY_DEPTHSTENCIL,
	eRS_DEFAUT
};

enum eRENDERTARGET_DEPTH_TYPE
{
	eRTDT_USE_DEPTH,
	eRTDT_USE_READONLY_DEPTH,
	eRTDT_NO_USE_DEPTH
};

struct RenderTargetDesc
{
	std::vector<Texture*>				  _RenderTargetList;
	eRENDERTARGET_DEPTH_TYPE			  _DepthType;
};

struct MeshBuffer
{
	ID3D11Buffer*		  vBuffer;
	ID3D11Buffer*		  vIBuffer;
};

class RenderDevice : public Singletone<RenderDevice>
{
public:
	RenderDevice();
	~RenderDevice();

public:
	static HWND								_hWnd;
	UINT									_Width;
	UINT									_Height;

private:
	ID3D11Device*							_pd3dDevice;
	ID3D11DeviceContext*					_pImmediateContext;
	IDXGISwapChain*							_pSwapChain;
	ID3D11RenderTargetView*					_pBackBufferRenderTargetView;
	ID3D11Texture2D*						_depthStencilBuffer;
	ID3D11DepthStencilView*					_depthStencilView;
	ID3D11DepthStencilView*					_depthStencilView_ReadOnly;
	ID3D11ShaderResourceView*				_depthStencilResourceView;

	UINT									_4xMsaaQuality;
	bool									_Enable4xMsaa;

	int										_fps;
	int										_ms;

private:
	FxInfo*									mPresentFX;
	RenderTargetDesc						mCurrentRenderTargetDesc;

public:
	HRESULT									InitDevice(HWND hWnd);
	HRESULT									ResizeDevice();

	void									SetRenderState(eRENDER_STATE renderstate);
	void									Begin(RenderTargetDesc& rendertargetDesc);
	void									End();
	HRESULT									BeginFX(std::string fxIndex, const int techIndex = 0);
	void									ApplyFX();
	HRESULT									EndFX();

	HRESULT									Present();
	void									Release();

	HWND									GetHandle() { return _hWnd; };
	ID3D11Device*							GetDevice();
	ID3D11DeviceContext*					GetContext();
	IDXGISwapChain*							GetSwapChain();
	ID3D11DepthStencilView*					GetDepthStencilView();
	ID3D11DepthStencilView*					GetReadOnlyDepthStencilView();
	ID3D11ShaderResourceView*				GetDepthStencilResourceView();
	ID3D11RenderTargetView*					GetBackBufferRenderTarget();
	ID3DX11EffectMatrixVariable*			GetVariableByName(const char* varname);
	ID3DX11EffectVariable*					GetRawVariableByName(const char* varname);
	ID3DX11EffectPass*						GetPassByIndex(const UINT index);

	void									DrawQuad();

	const UINT								GetWidth() { return _Width; };
	const UINT								GetHeight() { return _Height; };

	const UINT								GetFPS() { return _fps; };
	const UINT								GetMS() { return _ms; };
};
#pragma once
#include "stdafx.h"
#include "../CommonUtil/Singletone.h"
#include "../CommonUtil/Macro.h"
#include <d3d11_1.h>
#include "d3dx11effect.h"
#include "VertexDesc.h"
#include <string>
#include <unordered_map>

enum FX_ID {
	D3D11_DEFAULT_FX,
	FX_COUNT
};

struct FxInfo{
	ID3DX11Effect*			   _FxEffect;
	ID3DX11EffectTechnique*	   _FxTech;
	D3D11_INPUT_ELEMENT_DESC*  _InputDesc;
	ID3D11InputLayout*		   _InputLayer;
};

class FXManager : public Singletone<FXManager>
{
public:
	static std::string		fxFilePath;
	HRESULT					LoadFX();
	FxInfo*					GetFx(std::string fxIndex, int techIndex);
	HRESULT					CompileFXFile(const std::wstring FileName = L"");
	void					Release();

private:
	std::unordered_map<std::string, std::vector<FxInfo>>		_FxEffectHashMap;


};
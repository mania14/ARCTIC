#pragma once
#include "../GameEngine/Component.h"
#include <d3d11_1.h>

class Texture : public Component
{
public:
	const static COMPONENTIDTYPE COMPONENTID = TEXTURE_COMPONENT;

public:
	explicit Texture();
	virtual ~Texture();

public:
	virtual int				Init();
	virtual int				Release();
	COMPONENTIDTYPE GetComponentID() override {
		return COMPONENTID;
	};

public:
	std::string							m_strFileName;
	ID3D11ShaderResourceView*			m_pResourceView;
	ID3D11UnorderedAccessView*			m_pUnorderdView;
	ID3D11RenderTargetView*				m_pRenderTargetView;
	ID3D11Buffer*						m_pOutputBuffer;
};
#include "stdafx.h"
#include "Texture.h"

Texture::Texture()
	:m_pResourceView(nullptr),
	m_pUnorderdView(nullptr),
	m_pRenderTargetView(nullptr),
	m_pOutputBuffer(nullptr)
{

}

Texture::~Texture()
{

}

int Texture::Init()
{
	return 0;
}

int Texture::Release()
{
	if(nullptr != m_pResourceView) m_pResourceView->Release();
	if(nullptr != m_pUnorderdView) m_pUnorderdView->Release();
	if(nullptr != m_pRenderTargetView) m_pRenderTargetView->Release();
	if(nullptr != m_pOutputBuffer) m_pOutputBuffer->Release();
	return 0;
}

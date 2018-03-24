#include "FXManager.h"
#include "RenderDevice.h"
#include "d3dx11effect.h"
#include "d3dcompiler.h"
#include "../CommonUtil/StringUtil.h"
#include "../System/VertexDesc.h"
#include "FileSystem.h"
#include <functional>
#include <string>

HRESULT FXManager::LoadFX()
{
	_FxEffectHashMap.clear();
	
	std::vector<std::wstring> fxolist;
	FileSystem::GetDirectoryFileList(L"../Res/fxo/", fxolist, L"*.fxo");

	HRESULT hr = E_FAIL;

	for (auto iter = fxolist.begin(); iter != fxolist.end(); iter++)
	{
		ID3DX11Effect* pEffect = nullptr;

		std::wstring FileFullPath = L"../Res/fxo/";

		FileFullPath += FileSystem::GetFileName(*iter);
		FileFullPath += std::wstring(L".fxo");

		hr = D3DX11CreateEffectFromFile(FileFullPath.c_str(), 0, RenderDevice::This().GetDevice(), &pEffect);

		if (SUCCEEDED(hr))
		{
			std::string strFileName = Wide2Multi(FileSystem::GetFileName(*iter).c_str());
			//뒤쪽 개행문자를 지운다			

			D3DX11_EFFECT_DESC effectDesc;
			pEffect->GetDesc(&effectDesc);

			for (uint32_t techIdx = 0; techIdx < effectDesc.Techniques; ++techIdx)
			{
				D3DX11_TECHNIQUE_DESC techDesc;
				ID3DX11EffectTechnique* pTech = pEffect->GetTechniqueByIndex(techIdx);
				pTech->GetDesc(&techDesc);

				auto iter = _FxEffectHashMap.insert(std::pair<std::string, std::vector<FxInfo>>(techDesc.Name, std::vector<FxInfo>()));
				std::vector<FxInfo>& vecRefFxInfo = (iter.first)->second;
				vecRefFxInfo.resize(techDesc.Passes);

				for (uint32_t passIdx = 0; passIdx < techDesc.Passes; ++passIdx)
				{
					D3DX11_PASS_DESC passDesc;
					pTech->GetPassByIndex(passIdx)->GetDesc(&passDesc);

					vecRefFxInfo[passIdx]._FxEffect = pEffect;
					vecRefFxInfo[passIdx]._FxEffect->AddRef();
					vecRefFxInfo[passIdx]._FxTech = pTech;
					vecRefFxInfo[passIdx]._InputDesc = VertexDesc::This().GetElementDesc(techDesc.Name);
					vecRefFxInfo[passIdx]._InputLayer = nullptr;

					if (nullptr != vecRefFxInfo[passIdx]._InputDesc)
					{
						hr = RenderDevice::This().GetDevice()->CreateInputLayout(vecRefFxInfo[passIdx]._InputDesc, VertexDesc::This().GetElementSize(techDesc.Name) / sizeof(D3D11_INPUT_ELEMENT_DESC), passDesc.pIAInputSignature,
							passDesc.IAInputSignatureSize, &vecRefFxInfo[passIdx]._InputLayer);
					}
					else
					{
						hr = S_OK;
					}

					if (FAILED(hr))
					{
						MessageBoxA(NULL, "cant CreateInputLayout", "error", MB_OK);
						pEffect->Release();
						continue;
					}
				}
			}
			pEffect->Release();
		}
	}

	return hr;
}

FxInfo * FXManager::GetFx(std::string fxIndex, int techIndex)
{
	auto vecFxInfo = _FxEffectHashMap.find(fxIndex);

	if(vecFxInfo == _FxEffectHashMap.end())
	{
		MessageBoxA(NULL, "No FXEffect", "error", MB_OK);
	}

	return &(vecFxInfo->second[techIndex]);
}

HRESULT FXManager::CompileFXFile(const std::wstring FileName)
{
	HRESULT hr = E_FAIL;

	std::vector<std::wstring> fxlist;
	std::wstring WFileFullPath = L"../Res/fx/";
	if(FileName.length() > 0)
	{
		WFileFullPath += FileName;
		fxlist.push_back(WFileFullPath);
	}else
	{
		FileSystem::GetDirectoryFileList(WFileFullPath, fxlist, L"*.fx");
	}

	for (auto iter = fxlist.begin(); iter != fxlist.end(); ++iter)
	{
		while(true)
		{
			ID3D10Blob* ErrMessage;
			ID3D10Blob* Code;

			ErrMessage = 0;
			Code = 0;

			hr = D3DCompileFromFile((*iter).c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, 0, "fx_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &Code, &ErrMessage);

			if (FAILED(hr))
			{
				char* compileErrors;
				size_t bufferSize;

				compileErrors = (char*)(ErrMessage->GetBufferPointer());
				bufferSize = ErrMessage->GetBufferSize();

				char* err = new char[bufferSize + 1];
				ZeroMemory(err, bufferSize + 1);
				memcpy(err, compileErrors, bufferSize);

				// 컴파일 에러가 있음을 팝업 메세지로 알려줍니다.
				MessageBoxA(RenderDevice::This()._hWnd, err, "Compile shader", MB_OK);

				ErrMessage->Release();
			}
			else
			{
				FILE* pfile;

				std::string FileFullPath = "../Res/fxo/";
				FileFullPath += FileSystem::GetFileName(Wide2Multi((*iter).c_str()));
				FileFullPath += std::string(".fxo");

				fopen_s(&pfile, FileFullPath.c_str(), "wb");

				if (pfile)
				{
					fwrite(Code->GetBufferPointer(), 1, Code->GetBufferSize(), pfile);
				}

				fclose(pfile);

				Code->Release();
				break;
			}
		}
	}

	return hr;
}

void FXManager::Release()
{
	for (auto iter = _FxEffectHashMap.begin(); iter != _FxEffectHashMap.end(); ++iter)
	{
		for (int i = 0; i < (*iter).second.size(); ++i)
		{
			if(iter->second[i]._FxTech) iter->second[i]._FxTech->Release();
			if(iter->second[i]._InputLayer) iter->second[i]._InputLayer->Release();
			if(iter->second[i]._FxTech) iter->second[i]._FxEffect->Release();
		}
	}
	_FxEffectHashMap.clear();
}

#include "stdafx.h"
#include "../CommonUtil/Macro.h"
#include "Mesh.h"
#include "FBXUtill.h"

//FbxManager* FBXUtill::pManager = nullptr;
//FbxScene* FBXUtill::pScene = nullptr;

FBXUtill::FBXUtill()
{
	//Init();
}

FBXUtill::~FBXUtill()
{
	Release();
}

bool FBXUtill::Init()
{
	// Prepare the FBX SDK.
	pManager = FbxManager::Create();
	if (!pManager)
	{
		FBXSDK_printf("Error: Unable to create FBX Manager!\n");
		exit(1);
	}
	else FBXSDK_printf("Autodesk FBX SDK version %s\n", pManager->GetVersion());

	//Create an IOSettings object. This object holds all import/export settings.
	FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
	pManager->SetIOSettings(ios);

	//Load plugins from the executable directory (optional)
	FbxString lPath = FbxGetApplicationDirectory();
	pManager->LoadPluginsDirectory(lPath.Buffer());

	//Create an FBX scene. This object holds most objects imported/exported from/to files.
	pScene = FbxScene::Create(pManager, "My Scene");
	if (!pScene)
	{
		FBXSDK_printf("Error: Unable to create FBX scene!\n");
		exit(1);
	}

	return true;
}

bool FBXUtill::Release()
{
	if (pManager) pManager->Destroy();
	return true;
}

FbxScene* FBXUtill::ImportFBXScene(const char * FileName)
{
	int lFileMajor, lFileMinor, lFileRevision;
	int lSDKMajor, lSDKMinor, lSDKRevision;
	//int lFileFormat = -1;
	int i, lAnimStackCount;
	bool lStatus;

	// Get the file version number generate by the FBX SDK.
	FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

	// Create an importer.
	FbxImporter* lImporter = FbxImporter::Create(pManager, "");

	// Initialize the importer by providing a filename.
	const bool lImportStatus = lImporter->Initialize(FileName, -1, pManager->GetIOSettings());
	lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

	if (!lImportStatus)
	{
		FbxString error = lImporter->GetStatus().GetErrorString();
		MessageBoxA(NULL, error.Buffer(), "error", MB_OK);

		if (lImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
		{
			FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
			FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", FileName, lFileMajor, lFileMinor, lFileRevision);
		}

		return false;
	}

	FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);

	if (lImporter->IsFBX())
	{
		FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", FileName, lFileMajor, lFileMinor, lFileRevision);

		// From this point, it is possible to access animation stack information without
		// the expense of loading the entire file.

		FBXSDK_printf("Animation Stack Information\n");

		lAnimStackCount = lImporter->GetAnimStackCount();

		FBXSDK_printf("    Number of Animation Stacks: %d\n", lAnimStackCount);
		FBXSDK_printf("    Current Animation Stack: \"%s\"\n", lImporter->GetActiveAnimStackName().Buffer());
		FBXSDK_printf("\n");

		for (i = 0; i < lAnimStackCount; i++)
		{
			FbxTakeInfo* lTakeInfo = lImporter->GetTakeInfo(i);

			FBXSDK_printf("    Animation Stack %d\n", i);
			FBXSDK_printf("         Name: \"%s\"\n", lTakeInfo->mName.Buffer());
			FBXSDK_printf("         Description: \"%s\"\n", lTakeInfo->mDescription.Buffer());

			// Change the value of the import name if the animation stack should be imported 
			// under a different name.
			FBXSDK_printf("         Import Name: \"%s\"\n", lTakeInfo->mImportName.Buffer());

			// Set the value of the import state to false if the animation stack should be not
			// be imported. 
			FBXSDK_printf("         Import State: %s\n", lTakeInfo->mSelect ? "true" : "false");
			FBXSDK_printf("\n");
		}

		// Set the import states. By default, the import states are always set to 
		// true. The code below shows how to change these states.
		pManager->GetIOSettings()->SetBoolProp(IMP_FBX_MATERIAL, true);
		pManager->GetIOSettings()->SetBoolProp(IMP_FBX_TEXTURE, true);
		pManager->GetIOSettings()->SetBoolProp(IMP_FBX_LINK, true);
		pManager->GetIOSettings()->SetBoolProp(IMP_FBX_SHAPE, true);
		pManager->GetIOSettings()->SetBoolProp(IMP_FBX_GOBO, true);
		pManager->GetIOSettings()->SetBoolProp(IMP_FBX_ANIMATION, true);
		pManager->GetIOSettings()->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
	}

	// Import the scene.
	lStatus = lImporter->Import(pScene);
	lImporter->Destroy();

	return pScene;
}

void FBXUtill::ReadFBXUVData(FbxMesh* pFbxMesh, Mesh* pMesh)
{
	if (pFbxMesh->GetElementUVCount() < 1)
	{
		throw std::exception("Invalid Normal Number");
	}

	FbxGeometryElementUV* vertexUV = pFbxMesh->GetElementUV(0);
	
	switch (vertexUV->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
	{
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			for (int i = 0; i < pMesh->vecVertex.size(); ++i)
			{
				pMesh->vecVertex[i].Tex.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(i).mData[0]);
				pMesh->vecVertex[i].Tex.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(i).mData[1]);
			}
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			for (int i = 0; i < pMesh->vecVertex.size(); ++i)
			{
				int index = vertexUV->GetIndexArray().GetAt(i);
				pMesh->vecVertex[i].Tex.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(i).mData[0]);
				pMesh->vecVertex[i].Tex.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(i).mData[1]);
			}
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;
	}
	break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			UINT Idx = 0;
			for (int i = 0; i < pFbxMesh->GetPolygonCount(); ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					int ctrlIdx = pFbxMesh->GetPolygonVertex(i, j);
					
					pMesh->vecVertex[ctrlIdx].Tex.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(Idx).mData[0]);
					pMesh->vecVertex[ctrlIdx].Tex.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(Idx).mData[1]);
					++Idx;
				}
			}
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int polyIdx = 0;
			for (int i = 0; i < pFbxMesh->GetPolygonCount(); ++i)
			{
				int lPolygonSize = pFbxMesh->GetPolygonSize(i);
				for (int j = 0; j < lPolygonSize; ++j)
				{
					//if (Idx < vertexUV->GetIndexArray().GetCount())
					{
						int ctrlIdx = pFbxMesh->GetPolygonVertex(i, j);
						int index = pFbxMesh->GetTextureUVIndex(i, j);

						pMesh->vecVertex[ctrlIdx].Tex.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
						pMesh->vecVertex[ctrlIdx].Tex.y = 1.f - static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]);
						polyIdx++;
					}
				}
			}
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;
	}
}

void FBXUtill::ReadFBXNormalData(FbxGeometryElementNormal * pElement, acm::float3 * pData)
{
}

void FBXUtill::ReadFBXMaterialData(FbxMesh* pFbxMesh, Mesh* pMesh)
{
	if (pFbxMesh->GetElementMaterialCount() < 1)
	{
		throw std::exception("Invalid Normal Number");
	}

	FbxGeometryElementMaterial* vertexMaterial = pFbxMesh->GetElementMaterial(0);

	switch (vertexMaterial->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
	{
		switch (vertexMaterial->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			for (int i = 0; i < pMesh->vecVertex.size(); ++i)
			{
				//pMesh->vecVertex[i].TexID = vertexMaterial->GetDirectArray().GetAt(i).GetClassId();
			}
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			for (int i = 0; i < pMesh->vecVertex.size(); ++i)
			{
				int index = vertexMaterial->GetIndexArray().GetAt(i);
				//pMesh->vecVertex[i].TexID.x = static_cast<float>(vertexMaterial->GetDirectArray().GetAt(i).mData[0]);
			}
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;
	}
	break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexMaterial->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			UINT Idx = 0;
			for (int i = 0; i < pFbxMesh->GetPolygonCount(); ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					int ctrlIdx = pFbxMesh->GetPolygonVertex(i, j);
					//pMesh->vecVertex[ctrlIdx].TexID = static_cast<float>(vertexMaterial->GetDirectArray().GetAt(Idx).mData[0]);
					++Idx;
				}
			}
			break;
		}

		case FbxGeometryElement::eIndexToDirect:
		{
			int Idx = 0;
			for (int i = 0; i < pFbxMesh->GetPolygonCount(); ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					if (Idx < vertexMaterial->GetIndexArray().GetCount())
					{
						int index = vertexMaterial->GetIndexArray().GetAt(Idx);
						int ctrlIdx = pFbxMesh->GetPolygonVertex(i, j);
						//pMesh->vecVertex[ctrlIdx].TexID = static_cast<float>(vertexMaterial->GetDirectArray().GetAt(index).mData[0]);
						++Idx;
					}
				}
			}
			break;
		}

		default:
			throw std::exception("Invalid Reference");
		}
	break;

	case FbxGeometryElement::eByPolygon:
		{
			FbxLayerElementArrayTemplate<int>* pTexID;
			pFbxMesh->GetMaterialIndices(&pTexID);
			int Idx = 0;
			for (int i = 0; i < pFbxMesh->GetPolygonCount(); ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					int ctrlIdx = pFbxMesh->GetPolygonVertex(i, j);
					pMesh->vecVertex[ctrlIdx].TexID = pTexID->GetAt(i);
				}
			}
		}
	break;

	default:
		for (int i = 0; i < pMesh->vecVertex.size(); ++i)
		{
			pMesh->vecVertex[i].TexID = 0;
		}
	break;
	}
}

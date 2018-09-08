#include "stdafx.h"
#include "ToolTransformManager.h"
#include "../GameEngine/PickingManager.h"
#include "../GameEngine/Transform.h"
#include "../GameEngine/GameObject.h"
#include "../GameEngine/SceneManager.h"
#include "../GameEngine/CameraManager.h"
#include "../System/RenderDevice.h"
#include "../System/Inputmanager.h"

ToolTransformManager::ToolTransformManager()
	:CurrentMode(TOOL_TRANSFORM_POSITION),
	CurrentAxis(TOOL_TRANSFORM_AXIS_COUNT),
	_currentGameObject(nullptr)
{
	_isClicked = false;
}

ToolTransformManager::~ToolTransformManager()
{
	_pGizmo->pPositionGizmoBuffer->Release();
	_pGizmo->pLineGizmoBuffer->Release();
	delete _pGizmo;
}

void ToolTransformManager::Init()
{
	CreateGizmo();
}

void ToolTransformManager::Update()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8001)
	{
		_currentGameObject = nullptr;
	}

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8001)
	{
		BaseScene* pScene = SceneManager::This().GetCurrentScene();
		if (nullptr != pScene)
		{
			if (nullptr == _currentGameObject)
			{
				std::list<GameObject*> rayList;
				PickingManager::This().RayPick(pScene->getObjectList(BaseScene::OBJECT_STATIC), InputManager::This().GetMousePosX(), InputManager::This().GetMousePosY(), rayList);
				_currentGameObject = PickingManager::This().GetPickGameObject();
			}else			
			{
				Transform* pTransform = _currentGameObject->GetComponent<Transform>();

				float4x4 invMatViewProj = CameraManager::This().GetCurrentCameraViewProj().inverse_val();

				float vx = (float)InputManager::This().GetMousePosX() / (float)RenderDevice::This().GetWidth() * 2.0f - 1.f;
				float vy = (float)InputManager::This().GetMousePosY() / (float)RenderDevice::This().GetHeight() * -2.0f + 1.f;

				float3 vWorldPosition = acm::TransformCoord(pTransform->GetPosition(), CameraManager::This().GetCurrentCameraViewProj());
				float3 vNewRay = acm::TransformCoord(float3(vWorldPosition.x, vWorldPosition.y, vWorldPosition.z), invMatViewProj);
				//XMVECTOR vPosition = XMLoadFloat3(&pTransform->GetPosition());
				//XMVECTOR vWorldPosition = XMVector3TransformCoord(vPosition, CameraManager::This().GetCurrentCameraViewProj());
				//XMVECTOR vecNewRay = XMVector3TransformCoord(XMVectorSet(vx, vy, XMVectorGetZ(vWorldPosition), 1.0f), invMatViewProj);

				static float3 tempAxis[TOOL_TRANSFORM_AXIS_COUNT] = { float3(1, 0, 0), float3(0, 1, 0), float3(0, 0, 1) };
				acm::AABB box;
				float dist;
				float lineLength = 0.5f + 0.1f;
				float extents = 0.1f;
				float rotationWeight = 10.f;

				if( TOOL_TRANSFORM_COUNT != CurrentAxis)
				{					
					if (_isClicked)
					{
						switch(CurrentMode)
						{
							case TOOL_TRANSFORM_POSITION:
							{
								float3 position = pTransform->GetPosition();
								switch (CurrentAxis)
								{
								case TOOL_TRANSFORM_AXIS_X:
									position.x += acm::float3::dot(vNewRay - _vecOldRay, pTransform->GetRight());
									break;
								case TOOL_TRANSFORM_AXIS_Y:
									position.y += acm::float3::dot(vNewRay - _vecOldRay, pTransform->GetUp());
									break;
								case TOOL_TRANSFORM_AXIS_Z:
									position.z += acm::float3::dot(vNewRay - _vecOldRay, pTransform->GetLook());
									break;
								}
								pTransform->SetPosition(position);
								break;
							}
							case TOOL_TRANSFORM_ROTATE:
							{
								float3 rotate = pTransform->GetRotation();
								switch (CurrentAxis)
								{
								case TOOL_TRANSFORM_AXIS_X:
									rotate.x += acm::float3::dot(vNewRay - _vecOldRay, pTransform->GetRight()) * rotationWeight;
									break;
								case TOOL_TRANSFORM_AXIS_Y:
									rotate.y += acm::float3::dot(vNewRay - _vecOldRay, pTransform->GetUp()) * rotationWeight;
									break;
								case TOOL_TRANSFORM_AXIS_Z:
									rotate.z += acm::float3::dot(vNewRay - _vecOldRay, pTransform->GetLook()) * rotationWeight;
									break;
								}
								pTransform->SetRotation(rotate);
								break;
							}
							case TOOL_TRANSFORM_SCALE:
							{
								float3 scale = pTransform->GetScale();
								switch (CurrentAxis)
								{
								case TOOL_TRANSFORM_AXIS_X:
									scale.x += acm::float3::dot(vNewRay - _vecOldRay, pTransform->GetRight());
									break;
								case TOOL_TRANSFORM_AXIS_Y:
									scale.y += acm::float3::dot(vNewRay - _vecOldRay, pTransform->GetUp());
									break;
								case TOOL_TRANSFORM_AXIS_Z:
									scale.z += acm::float3::dot(vNewRay - _vecOldRay, pTransform->GetLook());
									break;
								}
								pTransform->SetRotation(scale);
								break;
							}
						}
					}else
					{
						_isClicked = true;
					}
				}else
				{					
					for (int i = TOOL_TRANSFORM_AXIS_X; i < TOOL_TRANSFORM_AXIS_COUNT; ++i)
					{
						box.Center = pTransform->GetPosition();

						box.Center.x += tempAxis[i].x * lineLength;
						box.Center.y += tempAxis[i].y * lineLength;
						box.Center.z += tempAxis[i].z * lineLength;

						box.Extents = float3(extents, extents, extents);

						if (PickingManager::This().RayPick(InputManager::This().GetMousePosX(), InputManager::This().GetMousePosY(), box, dist))
						{
							CurrentAxis = (PickAxis)i;
							break;
						}
					}
				}

				_vecOldRay = vNewRay;
			}
		}
	}
	else
	{
		_isClicked = false;
		CurrentAxis = TOOL_TRANSFORM_AXIS_COUNT;
	}
}

void ToolTransformManager::Render()
{
	if (nullptr != _currentGameObject)
	{
		RenderGizmo(PickingManager::This().GetPickGameObject()->GetComponent<Transform>());
	}
}

void ToolTransformManager::CreateGizmo()
{
	if (nullptr != _pGizmo)
	{
		delete _pGizmo;
	}

	_pGizmo = new Gizmo();

	//Create Gizmo
	if (_pGizmo->pPositionGizmoBuffer == nullptr && _pGizmo->pLineGizmoBuffer == nullptr)
	{
		float radious = 0.1f;

		std::vector<Vertex_Debug> vecLine;

		Vertex_Debug center;
		center.Pos = float3(0, 0, 0);
		vecLine.push_back(center);

		Vertex_Debug temp;
		temp.Pos = float3(0, 0, radious * 5);
		vecLine.push_back(temp);

		{
			_pGizmo->LineGizmoCount = (int)vecLine.size();
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(Vertex_Debug) * _pGizmo->LineGizmoCount;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA InitData;
			ZeroMemory(&InitData, sizeof(InitData));
			InitData.pSysMem = &vecLine[0];

			RenderDevice::This().GetDevice()->CreateBuffer(&bd, &InitData, &_pGizmo->pLineGizmoBuffer);
		}

		std::vector<Vertex_Debug> vecGizmo;
		for (int i = 0; i < 12; ++i)
		{
			Vertex_Debug temp1;
			temp1.Pos.x = sin(DEGREE_TO_RADIAN * 30 * i) * radious + vecLine[1].Pos.x;
			temp1.Pos.y = cos(DEGREE_TO_RADIAN * 30 * i) * radious + vecLine[1].Pos.y;
			temp1.Pos.z = 0 + vecLine[1].Pos.z;
			vecGizmo.push_back(temp1);

			Vertex_Debug temp2;
			temp2.Pos.x = sin(DEGREE_TO_RADIAN * 30 * (i + 1)) * radious + vecLine[1].Pos.x;
			temp2.Pos.y = cos(DEGREE_TO_RADIAN * 30 * (i + 1)) * radious + vecLine[1].Pos.y;
			temp2.Pos.z = 0 + vecLine[1].Pos.z;
			vecGizmo.push_back(temp2);

			Vertex_Debug temp3;
			temp3.Pos.x = 0 + vecLine[1].Pos.x;
			temp3.Pos.y = 0 + vecLine[1].Pos.y;
			temp3.Pos.z = radious * 3 + vecLine[1].Pos.z;
			vecGizmo.push_back(temp3);
		}

		{
			_pGizmo->PositionGizmoCount = (int)vecGizmo.size();
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(Vertex_Debug) * _pGizmo->PositionGizmoCount;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA InitData;
			ZeroMemory(&InitData, sizeof(InitData));
			InitData.pSysMem = &vecGizmo[0];

			RenderDevice::This().GetDevice()->CreateBuffer(&bd, &InitData, &(_pGizmo->pPositionGizmoBuffer));
		}

		float w2 = 0.1f;
		float h2 = 0.1f;
		float d2 = 0.1f;

		std::vector<Vertex_Debug> vertices =
		{
			{ float3(-w2, -h2, -d2 + vecLine[1].Pos.z)},
			{ float3(-w2, +h2, -d2 + vecLine[1].Pos.z)},
			{ float3(+w2, +h2, -d2 + vecLine[1].Pos.z)},
			{ float3(+w2, -h2, -d2 + vecLine[1].Pos.z)},

			{ float3(-w2, -h2, +d2 + vecLine[1].Pos.z)},
			{ float3(+w2, -h2, +d2 + vecLine[1].Pos.z)},
			{ float3(+w2, +h2, +d2 + vecLine[1].Pos.z)},
			{ float3(-w2, +h2, +d2 + vecLine[1].Pos.z)},

			{ float3(-w2, +h2, -d2 + vecLine[1].Pos.z)},
			{ float3(-w2, +h2, +d2 + vecLine[1].Pos.z)},
			{ float3(+w2, +h2, +d2 + vecLine[1].Pos.z)},
			{ float3(+w2, +h2, -d2 + vecLine[1].Pos.z)},

			{ float3(-w2, -h2, -d2 + vecLine[1].Pos.z)},
			{ float3(+w2, -h2, -d2 + vecLine[1].Pos.z)},
			{ float3(+w2, -h2, +d2 + vecLine[1].Pos.z)},
			{ float3(-w2, -h2, +d2 + vecLine[1].Pos.z)},

			{ float3(-w2, -h2, +d2 + vecLine[1].Pos.z)},
			{ float3(-w2, +h2, +d2 + vecLine[1].Pos.z)},
			{ float3(-w2, +h2, -d2 + vecLine[1].Pos.z)},
			{ float3(-w2, -h2, -d2 + vecLine[1].Pos.z)},

			{ float3(+w2, -h2, -d2 + vecLine[1].Pos.z)},
			{ float3(+w2, +h2, -d2 + vecLine[1].Pos.z)},
			{ float3(+w2, +h2, +d2 + vecLine[1].Pos.z)},
			{ float3(+w2, -h2, +d2 + vecLine[1].Pos.z)},
		};

		{
			_pGizmo->ScaleGizmoCount = (int)vertices.size();
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(Vertex_Debug) * _pGizmo->ScaleGizmoCount;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA InitData;
			ZeroMemory(&InitData, sizeof(InitData));
			InitData.pSysMem = &vertices[0];

			RenderDevice::This().GetDevice()->CreateBuffer(&bd, &InitData, &(_pGizmo->pScaleGizmoBuffer));
		}
	}
}

void ToolTransformManager::RenderGizmo(Transform* pTransform)
{
	RenderDevice::This().Begin("DebugTech");
	{
		// Draw Line
		UINT stride = sizeof(Vertex_Debug);
		UINT offset = 0;

		static float4 AxisColor[TOOL_TRANSFORM_AXIS_COUNT] = { float4(1, 0, 0, 1), float4(0, 1, 0, 1), float4(0, 0, 1, 1) };

		if (_isClicked)
		{
			AxisColor[CurrentAxis] = float4(1, 1, 1, 1);
		}

		float4x4 worldviewproj = pTransform->GetWorldMatrix() * CameraManager::This().GetCurrentCameraViewProj();

		{
			RenderDevice::This().GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			RenderDevice::This().GetContext()->OMSetDepthStencilState(RenderStates::This().NoUseDepthStencilDSS, 0);
			RenderDevice::This().GetContext()->IASetVertexBuffers(0, 1, &(_pGizmo->pLineGizmoBuffer), &stride, &offset);

			float4x4 matPosition;
			matPosition._11 = 1.f; matPosition._22 = 1.f; matPosition._33 = 1.f; matPosition._44 = 1.f;
			matPosition._41 = pTransform->GetPosition().x; matPosition._42 = pTransform->GetPosition().y; matPosition._43 = pTransform->GetPosition().z;

			float4x4 worldviewproj = matPosition * CameraManager::This().GetCurrentCameraViewProj();
			RenderDevice::This().GetVariableByName("gWorldViewProj")->SetMatrix(reinterpret_cast<float*>(&worldviewproj));
			RenderDevice::This().GetRawVariableByName("gColor")->AsVector()->SetRawValue(&AxisColor[TOOL_TRANSFORM_AXIS_Z], 0, sizeof(float3));
			RenderDevice::This().GetPassByIndex(0)->Apply(0, RenderDevice::This().GetContext());
			RenderDevice::This().GetContext()->Draw((_pGizmo->LineGizmoCount), 0);

			worldviewproj = MakeRotateMatrix(float3(0, DEGREE_TO_RADIAN * 90, 0)) * matPosition * CameraManager::This().GetCurrentCameraViewProj();
			RenderDevice::This().GetVariableByName("gWorldViewProj")->SetMatrix(reinterpret_cast<float*>(&worldviewproj));
			RenderDevice::This().GetRawVariableByName("gColor")->AsVector()->SetRawValue(&AxisColor[TOOL_TRANSFORM_AXIS_X], 0, sizeof(float3));
			RenderDevice::This().GetPassByIndex(0)->Apply(0, RenderDevice::This().GetContext());
			RenderDevice::This().GetContext()->Draw((_pGizmo->LineGizmoCount), 0);

			worldviewproj = MakeRotateMatrix(float3(DEGREE_TO_RADIAN * -90, 0, 0)) * matPosition * CameraManager::This().GetCurrentCameraViewProj();
			RenderDevice::This().GetVariableByName("gWorldViewProj")->SetMatrix(reinterpret_cast<float*>(&worldviewproj));
			RenderDevice::This().GetRawVariableByName("gColor")->AsVector()->SetRawValue(&AxisColor[TOOL_TRANSFORM_AXIS_Y], 0, sizeof(float3));
			RenderDevice::This().GetPassByIndex(0)->Apply(0, RenderDevice::This().GetContext());
			RenderDevice::This().GetContext()->Draw((_pGizmo->LineGizmoCount), 0);
		}

		// Draw Gizmo
		{
			RenderDevice::This().GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			RenderDevice::This().GetContext()->OMSetDepthStencilState(RenderStates::This().NoUseDepthStencilDSS, 0);
			RenderDevice::This().GetContext()->IASetVertexBuffers(0, 1, &(_pGizmo->pPositionGizmoBuffer), &stride, &offset);

			float4x4 matPosition;
			matPosition._11 = 1.f; matPosition._22 = 1.f; matPosition._33 = 1.f; matPosition._44 = 1.f;
			matPosition._41 = pTransform->GetPosition().x; matPosition._42 = pTransform->GetPosition().y; matPosition._43 = pTransform->GetPosition().z;

			float4x4 worldviewproj = matPosition * CameraManager::This().GetCurrentCameraViewProj();
			RenderDevice::This().GetVariableByName("gWorldViewProj")->SetMatrix(reinterpret_cast<float*>(&worldviewproj));
			RenderDevice::This().GetRawVariableByName("gColor")->AsVector()->SetRawValue(&AxisColor[TOOL_TRANSFORM_AXIS_Z], 0, sizeof(XMFLOAT3));
			RenderDevice::This().GetPassByIndex(0)->Apply(0, RenderDevice::This().GetContext());
			RenderDevice::This().GetContext()->Draw(_pGizmo->PositionGizmoCount, 0);

			worldviewproj = MakeRotateMatrix(float3(DEGREE_TO_RADIAN * -90, 0, 0)) * matPosition * CameraManager::This().GetCurrentCameraViewProj();
			RenderDevice::This().GetVariableByName("gWorldViewProj")->SetMatrix(reinterpret_cast<float*>(&worldviewproj));
			RenderDevice::This().GetRawVariableByName("gColor")->AsVector()->SetRawValue(&AxisColor[TOOL_TRANSFORM_AXIS_X], 0, sizeof(XMFLOAT3));
			RenderDevice::This().GetPassByIndex(0)->Apply(0, RenderDevice::This().GetContext());
			RenderDevice::This().GetContext()->Draw(_pGizmo->PositionGizmoCount, 0);

			worldviewproj = MakeRotateMatrix(float3(DEGREE_TO_RADIAN * -90, 0, 0)) * matPosition * CameraManager::This().GetCurrentCameraViewProj();
			RenderDevice::This().GetVariableByName("gWorldViewProj")->SetMatrix(reinterpret_cast<float*>(&worldviewproj));
			RenderDevice::This().GetRawVariableByName("gColor")->AsVector()->SetRawValue(&AxisColor[TOOL_TRANSFORM_AXIS_Y], 0, sizeof(XMFLOAT3));
			RenderDevice::This().GetPassByIndex(0)->Apply(0, RenderDevice::This().GetContext());
			RenderDevice::This().GetContext()->Draw(_pGizmo->PositionGizmoCount, 0);
		}

		//{
		//	RenderDevice::This().GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		//	RenderDevice::This().GetContext()->OMSetDepthStencilState(RenderStates::This().NoUseDepthStencilDSS, 0);
		//	RenderDevice::This().GetContext()->IASetVertexBuffers(0, 1, &(_pGizmo->pScaleGizmoBuffer), &stride, &offset);

		//	XMFLOAT4X4 matPosition;
		//	ZeroMemory(&matPosition, sizeof(XMFLOAT4X4));
		//	matPosition._11 = 1.f; matPosition._22 = 1.f; matPosition._33 = 1.f; matPosition._44 = 1.f;
		//	matPosition._41 = pTransform->GetPosition().x; matPosition._42 = pTransform->GetPosition().y; matPosition._43 = pTransform->GetPosition().z;

		//	XMMATRIX worldviewproj = DirectX::XMMatrixRotationRollPitchYaw(0, 0, 0) * XMLoadFloat4x4(&matPosition) * CameraManager::This().GetCurrentCameraViewProj();
		//	RenderDevice::This().GetVariableByName("gWorldViewProj")->SetMatrix(reinterpret_cast<float*>(&worldviewproj));
		//	RenderDevice::This().GetPassByIndex(0)->Apply(0, RenderDevice::This().GetContext());
		//	RenderDevice::This().GetContext()->Draw(_pGizmo->ScaleGizmoCount, 0);

		//	worldviewproj = DirectX::XMMatrixRotationRollPitchYaw(0, DEGREE_TO_RADIAN * 90, 0) * XMLoadFloat4x4(&matPosition) * CameraManager::This().GetCurrentCameraViewProj();
		//	RenderDevice::This().GetVariableByName("gWorldViewProj")->SetMatrix(reinterpret_cast<float*>(&worldviewproj));
		//	RenderDevice::This().GetPassByIndex(0)->Apply(0, RenderDevice::This().GetContext());
		//	RenderDevice::This().GetContext()->Draw(_pGizmo->ScaleGizmoCount, 0);

		//	worldviewproj = DirectX::XMMatrixRotationRollPitchYaw(DEGREE_TO_RADIAN * -90, 0, 0) * XMLoadFloat4x4(&matPosition) * CameraManager::This().GetCurrentCameraViewProj();
		//	RenderDevice::This().GetVariableByName("gWorldViewProj")->SetMatrix(reinterpret_cast<float*>(&worldviewproj));
		//	RenderDevice::This().GetPassByIndex(0)->Apply(0, RenderDevice::This().GetContext());
		//	RenderDevice::This().GetContext()->Draw(_pGizmo->ScaleGizmoCount, 0);
		//}
	}
	RenderDevice::This().End("DebugTech");

	RenderDevice::This().GetContext()->OMSetDepthStencilState(0, 0);
}


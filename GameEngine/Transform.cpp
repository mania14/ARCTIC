#include "stdafx.h"
#include "../CommonUtil/CommonDefine.h"
#include "Transform.h"
#include "../System/RenderDevice.h"
#include "CameraManager.h"

using namespace acm;

Transform::Transform()
	: _Position(float3(0,0,0))
	, _Scale(float3(1.f, 1.f, 1.f))
	, _Rotation(float3(0, 0, 0))
	, _Right(float3(1, 0, 0))
	, _Up(float3(0, 1, 0))
	, _Look(float3(0, 0, 1))
	, _Speed(1.f)
	, _Center(float3(0, 0, 0))
	, _Extents(float3(1, 1, 1))
{
	++_refCount[GetComponentID()];
}

Transform::~Transform()
{
	--_refCount[GetComponentID()];
}

int Transform::Init()
{
	return 0;
}

int Transform::Update()
{
	_Rotation.x = _Rotation.x >= 360.f ? 0 : _Rotation.x;
	_Rotation.y = _Rotation.y >= 360.f ? 0 : _Rotation.y;
	_Rotation.z = _Rotation.z >= 360.f ? 0 : _Rotation.z;
	
	float3 fRotation = _Rotation * DEGREE_TO_RADIAN;

	//XMVECTOR vRotation = XMLoadFloat3(&fRotation);
	//XMVECTOR vPosition = XMLoadFloat3(&_Position);

	float4x4 mRotation = MakeQuaternionToRotateMatrix(fRotation);
	_Right = float3(mRotation._11, mRotation._21, mRotation._31);
	_Up = float3(mRotation._12, mRotation._22, mRotation._32);
	_Look = float3(mRotation._13, mRotation._23, mRotation._33);

	_mWorld = mRotation;
	_mWorld._41 = _Position.x;
	_mWorld._42 = _Position.y;
	_mWorld._43 = _Position.z;
	_mWorld._44 = 1.f;

	return 0;
}

int Transform::Release()
{
	return 0;
}

void Transform::SetPosition(const float3 & position)
{
	_Position.x = position.x;
	_Position.y = position.y;
	_Position.z = position.z;
}

const float3& Transform::GetPosition()
{
	return _Position;
}

void Transform::SetScale(const float3 & scale)
{
	_Scale = scale;
}

const float3& Transform::GetScale()
{
	return _Scale;
}

void Transform::SetRotation(const float3 & rotation)
{
	_Rotation = rotation;
}

const float3& Transform::GetRotation()
{
	return _Rotation;
}

void Transform::SetLook(const float3 & look)
{
	_Look = look;

	// 벡터로부터 angle계산 다시 찾아서 공부하기!!!!

	float distance = sqrt(_Look.z * _Look.z + _Look.x * _Look.x);

	_Rotation.x = atan2f(_Look.y, distance) * RADIAN_TO_DEGREE * -1;	
	//_Rotation.z = atan2f(_Look.y, distance) * RADIAN_TO_DEGREE;
	_Rotation.y = atan2f(_Look.x, _Look.z) * RADIAN_TO_DEGREE;
}

const float3 & Transform::GetLook()
{
	return _Look;
}

const float3 & Transform::GetRight()
{
	return _Right;
}

const float3 & Transform::GetUp()
{
	return _Up;
}

const float4x4& Transform::GetWorldMatrix()
{
	return _mWorld;
}

//const float4x4 Transform::GetWorldMatrixInvTranspose()
//{
//	// Inverse-transpose is just applied to normals.  So zero out 
//	// translation row so that it doesn't get into our inverse-transpose
//	// calculation--we don't want the inverse-transpose of the translation. 
//	 
//	 
//	DirectX::acm::float4x4 A = _mWorld;
//	A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
//
//	DirectX::XMVECTOR det = DirectX::acm::float4x4Determinant(A);
//	return DirectX::acm::float4x4Transpose(acm::float4x4Inverse(&det, A));
//}

void Transform::RenderBox()
{
	std::vector<Vertex_Debug> tempBB;

	tempBB.resize(24);
	tempBB[0].Pos.x = _Center.x + _Extents.x;
	tempBB[0].Pos.y = _Center.y + _Extents.y;
	tempBB[0].Pos.z = _Center.z + _Extents.z;
	
	tempBB[1].Pos.x = _Center.x + -_Extents.x;
	tempBB[1].Pos.y = _Center.y + _Extents.y;
	tempBB[1].Pos.z = _Center.z + _Extents.z;

	tempBB[2] = tempBB[1];

	tempBB[3].Pos.x = _Center.x + -_Extents.x;
	tempBB[3].Pos.y = _Center.y + _Extents.y;
	tempBB[3].Pos.z = _Center.z + -_Extents.z;

	tempBB[4] = tempBB[3];

	tempBB[5].Pos.x = _Center.x + _Extents.x;
	tempBB[5].Pos.y = _Center.y + _Extents.y;
	tempBB[5].Pos.z = _Center.z + -_Extents.z;

	tempBB[6] = tempBB[5];
	tempBB[7] = tempBB[0];

	tempBB[8].Pos.x = _Center.x + _Extents.x;
	tempBB[8].Pos.y = _Center.y + -_Extents.y;
	tempBB[8].Pos.z = _Center.z + _Extents.z;

	tempBB[9].Pos.x = _Center.x + -_Extents.x;
	tempBB[9].Pos.y = _Center.y + -_Extents.y;
	tempBB[9].Pos.z = _Center.z + _Extents.z;

	tempBB[10] = tempBB[9];

	tempBB[11].Pos.x = _Center.x + -_Extents.x;
	tempBB[11].Pos.y = _Center.y + -_Extents.y;
	tempBB[11].Pos.z = _Center.z + -_Extents.z;

	tempBB[12] = tempBB[11];

	tempBB[13].Pos.x = _Center.x + _Extents.x;
	tempBB[13].Pos.y = _Center.y + -_Extents.y;
	tempBB[13].Pos.z = _Center.z + -_Extents.z;

	tempBB[14] = tempBB[13];
	tempBB[15] = tempBB[8];

	tempBB[16] = tempBB[0];
	tempBB[17] = tempBB[8];

	tempBB[18] = tempBB[1];
	tempBB[19] = tempBB[9];

	tempBB[20] = tempBB[3];
	tempBB[21] = tempBB[11];

	tempBB[22] = tempBB[5];
	tempBB[23] = tempBB[13];

	ID3D11Buffer*		  pBuffer;
	
	int Count = (int)tempBB.size();
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex_Debug) * Count;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &tempBB[0];

	RenderDevice::This().GetDevice()->CreateBuffer(&bd, &InitData, &pBuffer);
	

	RenderDevice::This().Begin("DebugTech");
	{
		// Draw Line
		UINT stride = sizeof(Vertex_Debug);
		UINT offset = 0;

		float4 color(1, 0, 0, 1);
		float4x4 worldviewproj = GetWorldMatrix() * CameraManager::This().GetCurrentCameraViewProj();

		{
			RenderDevice::This().GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			RenderDevice::This().GetContext()->OMSetDepthStencilState(RenderStates::This().NoUseDepthStencilDSS, 0);
			RenderDevice::This().GetVariableByName("gWorldViewProj")->SetMatrix(reinterpret_cast<float*>(&worldviewproj));
			RenderDevice::This().GetRawVariableByName("gColor")->AsVector()->SetRawValue(&color, 0, sizeof(float3));
			RenderDevice::This().GetContext()->IASetVertexBuffers(0, 1, &(pBuffer), &stride, &offset);
			RenderDevice::This().GetPassByIndex(0)->Apply(0, RenderDevice::This().GetContext());
			RenderDevice::This().GetContext()->Draw((Count), 0);
		}
	}
	RenderDevice::This().End("DebugTech");

	pBuffer->Release();
}
#pragma once
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <DirectXPackedVector.h>

namespace acm
{
	struct float2;
	struct float3;
	struct float4;
	struct float4x4;

	typedef DirectX::PackedVector::HALF half;

	static half FloatToHalf(float f)
	{
		return DirectX::PackedVector::XMConvertFloatToHalf(f);
	};

	static float HalfToFloat(half h)
	{
		return DirectX::PackedVector::XMConvertHalfToFloat(h);
	};

	struct half2 : DirectX::PackedVector::XMHALF2
	{

	};

	struct half4 : DirectX::PackedVector::XMHALF4
	{
		void setValue(const float f1, const float f2, const float f3, const float f4)
		{
			this->x = FloatToHalf(f1);
			this->y = FloatToHalf(f2);
			this->z = FloatToHalf(f3);
			this->w = FloatToHalf(f4);
		}
	};
	
	struct float2 : DirectX::XMFLOAT2
	{
		float2() {}
		float2(float _x, float _y) : XMFLOAT2(_x, _y) {}
		float2& operator= (const float2& Float2) { x = Float2.x; y = Float2.y; return *this; }
		float2 operator+ (const float2& Float2) {
			return float2(x + Float2.x, y + Float2.y);
		}
		float2& operator+= (const float2& Float2) {
			x += Float2.x; y += Float2.y;
			return *this;
		}
		float2 operator- (const float2& Float2) {
			return float2(x - Float2.x, y - Float2.y);
		}
		float2& operator-= (const float2& Float2) {
			x -= Float2.x; y -= Float2.y;
			return *this;
		}
		float2 operator* (const float2& Float2) {
			return float2(x * Float2.x, y * Float2.y);
		}
		float2& operator*= (const float2& Float2) {
			x *= Float2.x; y *= Float2.y;
			return *this;
		}
		float2 operator* (const float& Float) {
			return float2(x * Float, y * Float);
		}
		float2& operator*= (const float& Float) {
			x *= Float; y *= Float;
			return *this;
		}
		float2 operator/ (const float2& Float2) {
			return float2(x / Float2.x, y / Float2.y);
		}
		float2& operator/= (const float2& Float2) {
			x /= Float2.x; y /= Float2.y;
			return *this;
		}
		float2 operator/ (const float& Float) {
			return float2(x / Float, y / Float);
		}
		float2& operator/= (const float& Float) {
			x /= Float; y /= Float;
			return *this;
		}
	};

	struct float3 : DirectX::XMFLOAT3
	{
		float3() {}
		float3(float _x, float _y, float _z) : XMFLOAT3(_x, _y, _z) {}

		float3& operator= (const float3& Float3) { x = Float3.x; y = Float3.y; z = Float3.z; return *this; }
		float3 operator+ (const float3& Float3) {
			return float3(x + Float3.x, y + Float3.y, z + Float3.z);
		}
		float3& operator+= (const float3& Float3) {
			x += Float3.x; y += Float3.y; z += Float3.z;
			return *this;
		}
		float3 operator- (const float3& Float3) const {
			return float3(x - Float3.x, y - Float3.y, z - Float3.z);
		}
		float3& operator-= (const float3& Float3) {
			x -= Float3.x; y -= Float3.y; z -= Float3.z;
			return *this;
		}
		float3 operator* (const float3& Float3) {
			return float3(x * Float3.x, y * Float3.y, z + Float3.z);
		}
		float3& operator*= (const float3& Float3) {
			x *= Float3.x; y *= Float3.y; z *= Float3.z;
			return *this;
		}
		float3 operator* (const float& Float) const{
			return float3(x * Float, y * Float, z * Float);
		}
		float3& operator*= (const float& Float) {
			x *= Float; y *= Float; z *= Float;
			return *this;
		}
		float3 operator/ (const float3& Float3) {
			return float3(x / Float3.x, y / Float3.y, z / Float3.z);
		}
		float3& operator/= (const float3& Float3) {
			x /= Float3.x; y /= Float3.y; z /= Float3.z;
			return *this;
		}
		float3 operator/ (const float& Float) {
			return float3(x / Float, y / Float, z / Float);
		}
		float3& operator/= (const float& Float) {
			x /= Float; y /= Float; z /= Float;
			return *this;
		}
		static float dot(const float3& lhs, const float3& rhs) {
			return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
		}
		static float3 cross(const float3& lhs, const float3& rhs) {
			return float3((lhs.y * rhs.z) - (lhs.z * rhs.y), (lhs.z * rhs.x) - (lhs.x * rhs.z), (lhs.x * rhs.y) - (lhs.y * rhs.x));
		}
		float3 normalize_val()
		{
			float length = sqrtf(float3::dot(*this, *this));

			return float3(x / length, y / length, z / length);
		}
		void normalize()
		{
			float length = sqrtf(float3::dot(*this, *this));

			x /= length;
			y /= length;
			z /= length;
		}
		float length()
		{
			return sqrtf(float3::dot(*this, *this));
		}
		float lengthsq()
		{
			return float3::dot(*this, *this);
		}
	};

	struct float4 : DirectX::XMFLOAT4
	{
		/*union {
			float3 xyz;
			float  w;
		};*/
		float4::float4() {};
		float4::float4(float _x, float _y, float _z, float _w) : XMFLOAT4(_x, _y, _z, _w) {};
		float4 operator= (const float4& Float4) { x = Float4.x; y = Float4.y; z = Float4.z; w = Float4.w; return *this; }
		float4 operator+ (const float4& Float4) {
			return float4(x + Float4.x, y + Float4.y, z + Float4.z, w + Float4.w);
		}
		float4& operator+= (const float4& Float4) {
			x += Float4.x; y += Float4.y; z += Float4.z; w += Float4.w;
			return *this;
		}
		float4 operator- (const float4& Float4) {
			return float4(x - Float4.x, y - Float4.y, z - Float4.z, w - Float4.w);
		}
		float4& operator-= (const float4& Float4) {
			x -= Float4.x; y -= Float4.y; z -= Float4.z; w -= Float4.w;
			return *this;
		}
		float4 operator* (const float4& Float4) {
			return float4(x * Float4.x, y * Float4.y, z + Float4.z, w + Float4.w);
		}
		float4& operator*= (const float4& Float4) {
			x *= Float4.x; y *= Float4.y; z *= Float4.z; w *= Float4.w;
			return *this;
		}
		float4 operator* (const float& Float) {
			return float4(x * Float, y * Float, z * Float, w + Float);
		}
		float4& operator*= (const float& Float) {
			x *= Float; y *= Float; z *= Float; w *= Float;
			return *this;
		}
		float4 operator/ (const float4& Float4) {
			return float4(x / Float4.x, y / Float4.y, z / Float4.z, w / Float4.w);
		}
		float4& operator/= (const float4& Float4) {
			x /= Float4.x; y /= Float4.y; z /= Float4.z; z /= Float4.w;
			return *this;
		}
		float4 operator/ (const float& Float) {
			return float4(x / Float, y / Float, z / Float, w / Float);
		}
		float4& operator/= (const float& Float) {
			x /= Float; y /= Float; z /= Float; w /= Float;
			return *this;
		}
		static float dot(const float4& lhs, const float4& rhs) {
			return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
		}
		static float3 cross(const float4& lhs, const float4& rhs) {
			return float3((lhs.y * rhs.z) - (lhs.z * rhs.y), (lhs.z * rhs.x) - (lhs.x * rhs.z), (lhs.x * rhs.y) - (lhs.y * rhs.x));
		}
		float4 normalize_val()
		{
			float length = sqrtf((x * x) + (y * y) + (z * z));

			return float4(x / length, y / length, z / length, w);
		}
		void normalize()
		{
			float length = sqrtf((x * x) + (y * y) + (z * z));

			x /= length;
			y /= length;
			z /= length;
			w /= length;
		}
	};

	static const float Identity[16] =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	struct float4x4 : DirectX::XMFLOAT4X4
	{
		float4x4() : XMFLOAT4X4(Identity) {};
		float4x4(const float* pFloat4x4) : XMFLOAT4X4(pFloat4x4) {};
		float4x4& operator= (const float4x4& M)
		{ 
			m[0][0] = M.m[0][0]; m[0][1] = M.m[0][1]; m[0][2] = M.m[0][2]; m[0][3] = M.m[0][3];
			m[1][0] = M.m[1][0]; m[1][1] = M.m[1][1]; m[1][2] = M.m[1][2]; m[1][3] = M.m[1][3];
			m[2][0] = M.m[2][0]; m[2][1] = M.m[2][1]; m[2][2] = M.m[2][2]; m[2][3] = M.m[2][3];
			m[3][0] = M.m[3][0]; m[3][1] = M.m[3][1]; m[3][2] = M.m[3][2]; m[3][3] = M.m[3][3];
			
			return *this;
		}
		float4x4& operator*= (const float4x4& Float4x4)
		{
			using namespace DirectX;
			XMMATRIX X = XMMatrixMultiply(XMLoadFloat4x4(this), XMLoadFloat4x4(&Float4x4));
			XMStoreFloat4x4(this, X);
			return *this;
		}
		void inverse()
		{
			using namespace DirectX;
			XMMATRIX X = XMMatrixInverse(nullptr, XMLoadFloat4x4(this));
			XMStoreFloat4x4(this, X);
		}
		float4x4 inverse_val() const
		{
			using namespace DirectX;
			XMMATRIX X = XMMatrixInverse(nullptr, XMLoadFloat4x4(this));
			float4x4 temp;
			XMStoreFloat4x4(&temp, X);
			return temp;
		}
		void transpose()
		{
			using namespace DirectX;
			XMMATRIX X = XMMatrixTranspose(XMLoadFloat4x4(this));
			XMStoreFloat4x4(this, X);
		}
	};

	inline float4x4 operator* (const float4x4& M1, const float4x4& M2)
	{
		using namespace DirectX;
		XMMATRIX X = XMMatrixMultiply(XMLoadFloat4x4(&M1), XMLoadFloat4x4(&M2));

		float4x4 R;
		XMStoreFloat4x4(&R, X);
		return R;
	}

	inline float4x4 MakeLookAtLH(float3 EyePosition,float3 FocusPosition, float3 UpDirection)
	{
		using namespace DirectX;
		XMMATRIX view = XMMatrixLookAtLH(XMLoadFloat3(&EyePosition), XMLoadFloat3(&FocusPosition), XMLoadFloat3(&UpDirection));
		float4x4 result;
		XMStoreFloat4x4(&result, view);

		return result;
	}

	inline float4x4 MakePerspectiveFovLH(float FovAngleY,
		float AspectHByW,
		float NearZ,
		float FarZ)
	{
		using namespace DirectX;
		XMMATRIX proj = XMMatrixPerspectiveFovLH(FovAngleY, AspectHByW, NearZ, FarZ);
		float4x4 result;
		XMStoreFloat4x4(&result, proj);

		return result;
	}

	inline static float3 TransformCoord(const float3& f1, const float4x4& m1)
	{
		using namespace DirectX;
		XMVECTOR temp = XMVector3TransformCoord(XMLoadFloat3(&f1), XMLoadFloat4x4(&m1));
		float3 result;
		XMStoreFloat3(&result, temp);

		return result;
	}
	inline static float3 TransformNormal(const float3& f1, const float4x4& m1)
	{
		using namespace DirectX;
		XMVECTOR temp = XMVector3TransformNormal(XMLoadFloat3(&f1), XMLoadFloat4x4(&m1));
		float3 result;
		XMStoreFloat3(&result, temp);

		return result;
	}

	static float4x4 MakeReflectMatrix(const float3& q1)
	{
		using namespace DirectX;
		XMMATRIX temp = XMMatrixReflect(XMLoadFloat4(&float4(q1.x, q1.y, q1.z, 0.f)));
		float4x4 M;
		XMStoreFloat4x4(&M, temp);

		return M;
	}

	static float4x4 MakeRotateMatrix(const float3& q1)
	{
		using namespace DirectX;
		//XMVECTOR Q = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&q1));
		//XMVECTOR Q = XMQuaternionRotationRollPitchYaw(q1.x, q1.y, q1.z);
		//XMVECTOR Q = XMMatrixRotationRollPitchYaw(q1.x, q1.y, q1.z);
		float4x4 M;
		XMStoreFloat4x4(&M, XMMatrixRotationRollPitchYaw(q1.x, q1.y, q1.z));

		//XMStoreFloat4x4(&M, XMMatrixRotationRollPitchYaw(q1.x, q1.y, q1.z));
		return M;
	}

	static float AngleFromXY(float x, float y)
	{
		float theta = 0.0f;

		// Quadrant I or IV
		if (x >= 0.0f)
		{
			// If x = 0, then atanf(y/x) = +pi/2 if y > 0
			//                atanf(y/x) = -pi/2 if y < 0
			theta = atanf(y / x); // in [-pi/2, +pi/2]

			if (theta < 0.0f)
				theta += 2.0f*DirectX::XM_PI; // in [0, 2*pi).
		}

		// Quadrant II or III
		else
			theta = atanf(y / x) + DirectX::XM_PI; // in [0, 2*pi).

		return theta;
	}

	struct AABB : DirectX::BoundingBox
	{
		bool Intersects(float3 Origin, float3 Direction, float& Dist) const
		{
			using namespace DirectX;
			return __super::Intersects(XMLoadFloat3(&float3(Origin.x, Origin.y, Origin.z)), XMLoadFloat3(&float3(Direction.x, Direction.y, Direction.z)), Dist);
		}
	};

	struct int3 : DirectX::XMINT3
	{
		int3() {}
		int3(int _x, int _y, int _z) : XMINT3(_x, _y, _z) {}

		int3& operator= (const int3& Int3) { x = Int3.x; y = Int3.y; z = Int3.z; return *this; }
		int3 operator+ (const int3& Int3) {
			return int3(x + Int3.x, y + Int3.y, z + Int3.z);
		}
		int3& operator+= (const int3& Int3) {
			x += Int3.x; y += Int3.y; z += Int3.z;
			return *this;
		}
		int3 operator- (const int3& Int3) const {
			return int3(x - Int3.x, y - Int3.y, z - Int3.z);
		}
		int3& operator-= (const int3& Int3) {
			x -= Int3.x; y -= Int3.y; z -= Int3.z;
			return *this;
		}
		int3 operator* (const int3& Int3) {
			return int3(x * Int3.x, y * Int3.y, z + Int3.z);
		}
		int3& operator*= (const int3& Int3) {
			x *= Int3.x; y *= Int3.y; z *= Int3.z;
			return *this;
		}
		int3 operator* (const int& Int) const {
			return int3(x * Int, y * Int, z * Int);
		}
		int3& operator*= (const int& Int) {
			x *= Int; y *= Int; z *= Int;
			return *this;
		}
		int3 operator/ (const int3& Int3) {
			return int3(x / Int3.x, y / Int3.y, z / Int3.z);
		}
		int3& operator/= (const int3& Int3) {
			x /= Int3.x; y /= Int3.y; z /= Int3.z;
			return *this;
		}
		int3 operator/ (const int& Int) {
			return int3(x / Int, y / Int, z / Int);
		}
		int3& operator/= (const int& Int) {
			x /= Int; y /= Int; z /= Int;
			return *this;
		}
	};
}
//
//static DirectX::XMMATRIX InverseTranspose(DirectX::CXMMATRIX M)
//{
//	// Inverse-transpose is just applied to normals.  So zero out 
//	// translation row so that it doesn't get into our inverse-transpose
//	// calculation--we don't want the inverse-transpose of the translation.
//	DirectX::XMMATRIX A = M;
//	A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
//
//	DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(A);
//	return DirectX::XMMatrixTranspose(XMMatrixInverse(&det, A));
//}
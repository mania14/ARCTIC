#pragma once
#include <DirectXMath.h>

namespace ACMATH
{
	struct float2
	{
		float x;
		float y;
		float z;

		float2() {}
		float2(float _x, float _y) : x(_x), y(_y) {}
		float2& operator= (const float2& Float3) { x = Float3.x; y = Float3.y; return *this; }
		float3& operator+ (const float3& Float3) {
			x += Float3.x;
			x += Float3.y;
			x += Float3.z;
		}
	};

	struct float3
	{
		float x;
		float y;
		float z;

		float3() {}
		float3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

		float3& operator= (const float3& Float3) { x = Float3.x; y = Float3.y; z = Float3.z; return *this; }
		float3& operator+ (const float3& Float3) { x += Float3.x; y += Float3.y; z += Float3.z; }
		float3& operator- (const float3& Float3) { x -= Float3.x; y -= Float3.y; z -= Float3.z; }
		float3& operator* (const float& Float) { x *= Float; y *= Float; z *= Float; }
		float3& operator/ (const float& Float) { x /= Float; y /= Float; z /= Float; }
		float3& operator* (const float& Float3) { x *= Float3.x; y *= Float3.y; z *= Float3.z; }
		static float dot(const float3& lhs, const float3& rhs) {
			return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
		}
		static float3 cross(const float3& lhs, const float3& rhs) {
			return float3((lhs.y * rhs.z) - (lhs.z * rhs.y), (lhs.z * rhs.x) - (lhs.x * rhs.z), (lhs.x * rhs.y) - (lhs.y * rhs.x));
		}
	};

	struct float4
	{
		float x;
		float y;
		float z;
		float w;

		float4() {}
		float4(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

		float4& operator= (const float4& Float4) { x = Float4.x; y = Float4.y; z = Float4.z; w = Float4.w; return *this; }
		float4& operator+ (const float4& Float4) { x += Float4.x; y += Float4.y; z += Float4.z; w += Float4.w; }
		float4& operator- (const float4& Float4) { x -= Float4.x; y -= Float4.y; z -= Float4.z; w -= Float4.w; }
		float4& operator* (const float& Float) { x *= Float; y *= Float; z *= Float; w *= Float; }
		float4& operator/ (const float& Float) { x /= Float; y /= Float; z /= Float; w /= Float; }
		float4& operator* (const float& Float4) { x *= Float3.x; y *= Float3.y; z *= Float3.z; w *= Float3.w }
	};

	//__declspec(align(16)) struct float3x3
	//{
	//	union
	//	{
	//		struct
	//		{
	//			float _m11; float _m12; float _m13;
	//			float _m21; float _m22; float _m23;
	//			float _m31; float _m32; float _m33;
	//		};
	//		float _m[3][3];
	//		float _m[3][3];
	//	};

	//	float3x3() {}
	//	float3x3(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33) : _m11(m11), _m12(m12), _m13(m13), _m21(m21), _m22(m22), _m23(m23), _m31(m31), _m32(m32), _m33(m33) {}

	//	float3x3& operator= (const float3x3& Float3x3) { _m11 = Float3x3._m11; _m12 = Float3x3._m12; _m13 = Float3x3._m13; _m21 = Float3x3._m21; _m22 = Float3x3._m22; _m23 = Float3x3._m23; _m31 = Float3x3._m31; _m32 = Float3x3._m32; _m33 = Float3x3._m33; return *this; }
	//	//float3x3& operator* (const float& Float) { x *= Float; y *= Float; z *= Float; w *= Float; }
	//	//float3x3& operator/ (const float& Float) { x /= Float; y /= Float; z /= Float; w /= Float; }
	//	float3x3& operator* (const float3x3& Float3x3)
	//	{
	//		__m128 m0 = _mm_load_ps(_m[0]);
	//		_mm_add
	//			_mm_store_ps(_m[0], m0);
	//		__m128 m1 = _mm_load_ps(_m[1]);
	//		__m128 m2 = _mm_load_ps(_m[2]);


	//	}
	//}

	__declspec(align(16)) struct float4x4
	{
		union
		{
			struct
			{
				float _m11; float _m12; float _m13; float _m14;
				float _m21; float _m22; float _m23; float _m24;
				float _m31; float _m32; float _m33; float _m34;
				float _m41; float _m42; float _m43; float _m44;
			};
			float _m[16];
		}; 

		float4x4() 
		{
			static const float identity[16] =
			{
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1 
			};

			memcpy(_m, identity, sizeof(float4x4));
		}
		float4x4(float* pFloat4x4) { memcpy(_m, pFloat4x4, sizeof(float4x4)); }
		float4x4& operator= (const float4x4& Float4x4) { 
			memcpy(_m, Float4x4._m, sizeof(float4x4));
			return *this; }
		//float3x3& operator* (const float& Float) { x *= Float; y *= Float; z *= Float; w *= Float; }
		//float3x3& operator/ (const float& Float) { x /= Float; y /= Float; z /= Float; w /= Float; }
		float4x4& operator* (const float4x4& Float4x4)
		{
			__m128 row1 = _mm_load_ps(&_m[0]);
			__m128 row2 = _mm_load_ps(&_m[4]);
			__m128 row3 = _mm_load_ps(&_m[8]);
			__m128 row4 = _mm_load_ps(&_m[12]);
			for (int i = 0; i < 4; i++) {
				__m128 brod1 = _mm_set1_ps(Float4x4._m[4 * i + 0]);
				__m128 brod2 = _mm_set1_ps(Float4x4._m[4 * i + 1]);
				__m128 brod3 = _mm_set1_ps(Float4x4._m[4 * i + 2]);
				__m128 brod4 = _mm_set1_ps(Float4x4._m[4 * i + 3]);
				__m128 row = _mm_add_ps(
					_mm_add_ps(
						_mm_mul_ps(brod1, row1),
						_mm_mul_ps(brod2, row2)),
					_mm_add_ps(
						_mm_mul_ps(brod3, row3),
						_mm_mul_ps(brod4, row4)));
				_mm_store_ps(&_m[4 * i], row);
			}
		}
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

	static DirectX::XMMATRIX InverseTranspose(DirectX::CXMMATRIX M)
	{
		// Inverse-transpose is just applied to normals.  So zero out 
		// translation row so that it doesn't get into our inverse-transpose
		// calculation--we don't want the inverse-transpose of the translation.
		DirectX::XMMATRIX A = M;
		A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(A);
		return DirectX::XMMatrixTranspose(XMMatrixInverse(&det, A));
	}
}
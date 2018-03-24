#pragma once
#include <DirectXMath.h>

namespace ACMATH
{
	struct FLOAT3 : DirectX::XMFLOAT3
	{
		const XMFLOAT3& operator+ (const XMFLOAT3& Float3) {
			x += Float3.x;
			x += Float3.y;
			x += Float3.z;
		}
	};

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
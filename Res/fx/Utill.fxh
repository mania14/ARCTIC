static const int firstOctave = 3;
static const int octaves = 8;
static const float persistence = 0.6;

//Not able to use bit operator like <<, so use alternative noise function from YoYo
//
//https://www.shadertoy.com/view/Mls3RS
//
//And it is a better realization I think

float noise(int x, int y)
{
	float fx = float(x);
	float fy = float(y);

	return 2.0 * frac(sin(dot(float2(fx, fy), float2(12.9898, 78.233))) * 43758.5453) - 1.0;
}

float smoothNoise(int x, int y)
{
	return noise(x, y) / 4.0 + (noise(x + 1, y) + noise(x - 1, y) + noise(x, y + 1) + noise(x, y - 1)) / 8.0 + (noise(x + 1, y + 1) + noise(x + 1, y - 1) + noise(x - 1, y + 1) + noise(x - 1, y - 1)) / 16.0;
}

float COSInterpolation(float x, float y, float n)
{
	float r = n*3.1415926;
	float f = (1.0 - cos(r))*0.5;
	return x*(1.0 - f) + y*f;

}

float InterpolationNoise(float x, float y)
{
	int ix = int(x);
	int iy = int(y);
	float fracx = x - float(int(x));
	float fracy = y - float(int(y));

	float v1 = smoothNoise(ix, iy);
	float v2 = smoothNoise(ix + 1, iy);
	float v3 = smoothNoise(ix, iy + 1);
	float v4 = smoothNoise(ix + 1, iy + 1);

	float i1 = COSInterpolation(v1, v2, fracx);
	float i2 = COSInterpolation(v3, v4, fracx);

	return COSInterpolation(i1, i2, fracy);

}

float PerlinNoise2D(float x, float y)
{
	float sum = 0.0;
	float frequency = 0.0;
	float amplitude = 0.0;
	for (int i = firstOctave; i<octaves + firstOctave; i++)
	{
		frequency = pow(2.0, float(i));
		amplitude = pow(persistence, float(i));
		sum = sum + InterpolationNoise(x*frequency, y*frequency)*amplitude;
	}

	return sum;
}

float ConvertZToLinearDepth(float depth ,float4 Perspective)
{
	float linearDepth = Perspective.z / (depth + Perspective.w);
	return linearDepth;
}

float3 CalcWorldPos(float2 csPos, float depth, float4x4 ViewInv, float4 Perspective)
{
	float4 position;

	position.xy = csPos.xy * Perspective.xy * depth;
	position.z = depth;
	position.w = 1.0;

	return mul(position, ViewInv).xyz;
}

void ClacNormalMap(float3 TBN, float3 wolrdTangent, float3 worldBinormal, float3 worldNormal, out float3 normal)
{
	float3 nTBN = normalize(TBN * 2 - 1);
	float3x3 worldTBN = float3x3(normalize(wolrdTangent), normalize(worldBinormal), normalize(worldNormal));
	worldTBN = transpose(worldTBN);
	normal = mul(worldTBN, nTBN);
}

void calcTBFromN(float3 normal, out float3 tangent, out float3 binormal)
{
	const float3 vecUp = float3(0, 1, 0);
	const float3 vecLook = float3(0, 0, 1);

	float3 tangentUp = cross(normal, vecUp);
	float3 tangentLook = cross(normal, vecLook);

	tangent = length(tangentUp) > length(tangentLook) ? tangentUp : tangentLook;
	tangent = normalize(tangent);
	binormal = cross(tangent, normal);
	binormal = normalize(binormal);
}
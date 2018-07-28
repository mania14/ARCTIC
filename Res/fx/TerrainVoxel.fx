#include "DefaultLightFunc.fx"
#include "VoxelTerrainLookUpTable.fxh"

static float3 up = { 0.0f, 1.0f, 0.0f };

cbuffer cbPerFrame
{
	Light gDirLight;
	PointLight gPointLight;
	SpotLight gSpotLight;
	float3 gEyePosW;
};

cbuffer cbPerTerrain
{
	float4x4 gViewProj;
	Material gMaterial;
};

struct VertexIn
{
	float3 VoxelInfo : VOXELINFO;
};

struct GeoOut
{
	float4 PosH		: SV_POSITION;
	float3 PosW		: POSITION;
	float3 Normal   : NORMAL;
	float2 Tex		: TEXCOORD;
	uint   PrimeID	: SV_PrimitiveID;
};

// //Texture2DArray gTreeTexArray;
// Texture2DArray gDiffuseMap;

Texture3D volumeTex;
SamplerState volumeTexSampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

const float3 volumeTexWeight[8] = 
{
	float3(0, 0, 0),
	float3(1, 0, 0),	
	float3(1, 1, 0),
	float3(0, 1, 0),
	float3(0, 0, 1),
	float3(1, 0, 1),
	float3(1, 1, 1),
	float3(0, 1, 1)
};

VertexIn VS(VertexIn IN)
{
	VertexIn OUT;

	OUT.VoxelInfo = IN.VoxelInfo;

	return OUT;
}

float calcDensityLerpValue( float density1, float density2 )
{
	return density1 / (max(density1, 0) + max(density2, 0));
}

float3 calcNormal()
{
	float aroundDensity[6];

	float3 accNormal;

	return normalize(accNormal);
}

[maxvertexcount(15)]
void GS(point VertexIn gIn[1], uint primeID : SV_PrimitiveID, inout TriangleStream<GeoOut> triStream)
{
	half3 normal[8];
	half  density[8];

	for (int samplerIdx = 0; samplerIdx < 8; ++samplerIdx)
	{
		half4 voxelInfo = volumeTex.SampleLevel(volumeTexSampler, (gIn[0].VoxelInfo.xyz + volumeTexWeight[samplerIdx]) / 33.f, 0);
		normal[samplerIdx] = voxelInfo.xyz;
		density[samplerIdx] = voxelInfo.w;
	}

	GeoOut gout;

	unsigned int meshIdx = ((unsigned int)(density[0] > 0) | (unsigned int)(density[1] > 0) << 1 | (unsigned int)(density[2] > 0) << 2 | (unsigned int)(density[3] > 0) << 3 | (unsigned int)(density[4] > 0) << 4 | (unsigned int)(density[5] > 0) << 5 | (unsigned int)(density[6] > 0) << 6 | (unsigned int)(density[7] > 0) << 7);
	meshIdx = clamp(meshIdx, 0, 255);

	int triStripCount = 0;
	for (int polyIdx = 0; polyIdx < 15; ++polyIdx)
	{
		unsigned int lookUpIndex = voxelTerrainLookUpTable[meshIdx][polyIdx];
		if (lookUpIndex < 0)
		{
			break;
		}

		unsigned int voxelIndex0 = voxelTerrainIndexLookUpTable[lookUpIndex][0];
		unsigned int voxelIndex1 = voxelTerrainIndexLookUpTable[lookUpIndex][1];

		gout.PosW = lerp(voxelTerrainPosLookUpTable[voxelIndex0], voxelTerrainPosLookUpTable[voxelIndex1], calcDensityLerpValue(density[voxelIndex0], density[voxelIndex1]));
		gout.PosW += gIn[0].VoxelInfo.xyz;
		gout.PosH = mul(float4(gout.PosW, 1.f), gViewProj);



		gout.Normal.x = (float)normal[0].x;
		gout.Normal.y = (float)normal[0].y;
		gout.Normal.z = (float)density[0];
		gout.Tex = float2(1, 1);
		gout.PrimeID = primeID;

		triStream.Append(gout);
		triStripCount++;

		if( (triStripCount % 3) == 0 )
		{
			triStream.RestartStrip();
		}
	}

	//float3 look = gEyePosW - gIn[0].VoxelInfo;
	//look.y = 0.0f;
	//look = normalize(look);
	//float3 right = cross(up, look);

	//float4 v[4];
	//v[0] = float4(gIn[0].VoxelInfo + 3 * right - 3 * up, 1.0f);
	//v[1] = float4(gIn[0].VoxelInfo + 3 * right + 3 * up, 1.0f);
	//v[2] = float4(gIn[0].VoxelInfo - 3 * right - 3 * up, 1.0f);
	//v[3] = float4(gIn[0].VoxelInfo - 3 * right + 3 * up, 1.0f);

	//GeoOut gout;
	//[unroll]
	//for (int j = 0; j < 4; ++j)
	//{
	//	gout.PosH = mul(v[j], gViewProj);
	//	gout.PosW = v[j].xyz;
	//	//gout.Normal = look;
	//	gout.Normal.x = 1.f;
	//	gout.Normal.y = 1.f;
	//	gout.Normal.z = 1.f;
	//	gout.Tex = float2(1,1);
	//	gout.PrimeID = primeID;

	//	triStream.Append(gout);
	//}
	
}

float4 PS(GeoOut IN) : SV_Target
{
	return float4(IN.Normal.zzz, 1.f);
	IN.Normal = normalize(IN.Normal);

	float3 toEyeW = normalize(gEyePosW - IN.PosW);

	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float4 A, D, S;

	CalcDirectionalLight(gMaterial, gDirLight, IN.Normal, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	specular += S;

	//CalcPointLight(gMaterial, gPointLight, IN.PosW, IN.Normal, toEyeW, A, D, S);
	//ambient += A;
	//diffuse += D;
	//specular += S;

	//float4 texColor = gDiffuseMap.Sample(DefaultSampler, IN.Tex);
	float3 uvw = float3(IN.Tex, IN.PrimeID % 4);
	float4 texColor = float4(1, 0, 0, 1);

	float4 litColor = texColor * (ambient + diffuse) + specular;
	litColor.a = texColor.a;

	//clip(texColor.a - 0.1f);

	return float4(1,1,1, 1.f);
}

technique11 TerrainVoxelTech
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(CompileShader(gs_5_0, GS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}
#include "Common.fxh"
#include "VoxelTerrainLookUpTable.fxh"
#include "Encode.fxh"

static float3 up = { 0.0f, 1.0f, 0.0f };

cbuffer cbPerTerrain
{
	float4x4 gViewProj;
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
	float3 Tangent  : TANGENT;
	float3 BiNormal : BINORMAL;
	float2 Tex		: TEXCOORD;
	uint   PrimeID	: SV_PrimitiveID;
};

Texture2D texDiffuse;
Texture2D texNormal;
Texture3D texVolume;
SamplerState volumeSampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
};
SamplerState diffuseSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};
SamplerState normalSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

int3 iSectorIndex;

VertexIn VS(VertexIn IN)
{
	VertexIn OUT;

	OUT.VoxelInfo = IN.VoxelInfo;

	return OUT;
}

float calcDensityLerpValue( float density1, float density2 )
{
	return saturate( abs(density1) / (abs(density1) + abs(density2)) );
}

float2 calcUV(float3 pos, float3 tangent, float3 binormal)
{
	float u = (dot(pos, tangent) + 1 ) * 0.5f;
	float v = (dot(pos, binormal) + 1 ) * 0.5f;

	//float u = asin(pos.x) / 3.14159f + 0.5f;
	//float v = asin(pos.y) / 3.14159f + 0.5f;
	
	return float2(u, v);
}

[maxvertexcount(15)]
void GS(point VertexIn gIn[1], uint primeID : SV_PrimitiveID, inout TriangleStream<GeoOut> triStream)
{
	half3 normal[8];
	half  density[8];

	for (int samplerIdx = 0; samplerIdx < 8; ++samplerIdx)
	{
		half4 voxelInfo = texVolume.SampleLevel(volumeSampler, (gIn[0].VoxelInfo.xyz + voxelTerrainPosLookUpTable[samplerIdx]) / g_VoxelSize, 0);
		normal[samplerIdx] = voxelInfo.xyz;
		density[samplerIdx] = (voxelInfo.w * 2) - 1;
	}

	unsigned int meshIdx = ((unsigned int)(density[0] > 0) | (unsigned int)(density[1] > 0) << 1 | (unsigned int)(density[2] > 0) << 2 | (unsigned int)(density[3] > 0) << 3 | (unsigned int)(density[4] > 0) << 4 | (unsigned int)(density[5] > 0) << 5 | (unsigned int)(density[6] > 0) << 6 | (unsigned int)(density[7] > 0) << 7);
	
	if(meshIdx == 0 || meshIdx == 255)
		return;
	
	meshIdx = clamp(meshIdx, 0, 255);

	GeoOut gout[3] = { (GeoOut)0, (GeoOut)0, (GeoOut)0 };
	int triStripCount = 0;
	for (int polyIdx = 0; polyIdx < 15; ++polyIdx)
	{
		unsigned int lookUpIndex = voxelTerrainLookUpTable[meshIdx][polyIdx];
		if (lookUpIndex < 0)
		{
			break;
		}

		// 복셀의 위치, extraNoraml 계산은 각각 따로 합니다.
		{
			unsigned int voxelIndex0 = voxelTerrainIndexLookUpTable[lookUpIndex][0];
			unsigned int voxelIndex1 = voxelTerrainIndexLookUpTable[lookUpIndex][1];
			
			float lerpDensity = calcDensityLerpValue(density[voxelIndex0], density[voxelIndex1]);
			gout[triStripCount].PosW = lerp(voxelTerrainPosLookUpTable[voxelIndex0], voxelTerrainPosLookUpTable[voxelIndex1], lerpDensity);
			//gout[triStripCount].Tex = lerp(voxelTerrainTexLookUpTable[voxelIndex0][0], voxelTerrainTexLookUpTable[voxelIndex1][1], lerpDensity);
			//gout[triStripCount].Tex = float2(0,0);

			// calc extra normal (추후에 다시 체크해봐야됨)
			// {
				// float  extraDensity[2];
				// float3 extraNormal[2];

				// extraDensity[0] = texVolume.SampleLevel(volumeSampler, (gIn[0].VoxelInfo.xyz + gout[triStripCount].PosW + voxelTerrainExtraDensityLookUpTable[lookUpIndex][0]) / g_VoxelSize, 0).w * 2 - 1;
				// extraDensity[1] = texVolume.SampleLevel(volumeSampler, (gIn[0].VoxelInfo.xyz + gout[triStripCount].PosW + voxelTerrainExtraDensityLookUpTable[lookUpIndex][1]) / g_VoxelSize, 0).w * 2 - 1;
				
				// float whichDensity = density[voxelIndex0] < 0 ? density[voxelIndex0] : density[voxelIndex1];

				// extraNormal[0] = normalize(voxelTerrainExtraDensityLookUpTable[lookUpIndex][0]) * calcDensityLerpValue(whichDensity, extraDensity[0]);
				// extraNormal[1] = normalize(voxelTerrainExtraDensityLookUpTable[lookUpIndex][1]) * calcDensityLerpValue(whichDensity, extraDensity[1]);

				// gout[triStripCount].Normal = extraNormal[0] + extraNormal[1];
			// }
			gout[triStripCount].PrimeID = primeID;

			triStripCount++;
		}

		if( triStripCount >= 3 )
		{
			// 복셀의 mainNormal, 월드 계산은 삼각형이 완성될때 마다 합니다.
			float3 mainNormal = normalize(cross(gout[2].PosW - gout[0].PosW, gout[1].PosW - gout[0].PosW));

			for (int gIdx = 0; gIdx < 3; ++gIdx)
			{
				// 보조 노멀은 나중에
				//gout[gIdx].Normal = normalize(mainNormal + gout[gIdx].Normal);
				gout[gIdx].Normal = normalize(mainNormal);
				calcTBFromN(gout[gIdx].Normal, gout[gIdx].Tangent, gout[gIdx].BiNormal);
				gout[gIdx].Tex = calcUV(gout[gIdx].PosW, gout[gIdx].Tangent, gout[gIdx].BiNormal);
				gout[gIdx].PosW += gIn[0].VoxelInfo.xyz;
				gout[gIdx].PosW += iSectorIndex.xyz * (g_VoxelSize - 1);
				gout[gIdx].PosH = mul(float4(gout[gIdx].PosW, 1.f), gViewProj);
				triStream.Append(gout[gIdx]);
			}
		
			triStream.RestartStrip();
			triStripCount = 0;
		}
	}
}

PS_GBUFFER_OUT PS(GeoOut IN)
{
	//float3 uvw = float3(IN.Tex, IN.TexID);
	float3 normal = IN.Normal;
	ClacNormalMap(texNormal.Sample(normalSampler, IN.Tex).xyz, IN.Tangent, IN.BiNormal, IN.Normal, normal);

	return PackGBufferFunc(texDiffuse.Sample(diffuseSampler, IN.Tex).xyz, normal, 1.f, 1.f);
}

technique11 TerrainVoxelTech
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(CompileShader(gs_5_0, GS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
		
		SetBlendState( NoBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetRasterizerState(CCW);
		SetDepthStencilState(UseDepth, 0);
	}
}
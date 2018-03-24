static const float3 TerrainVoxelVertexLookUpTable[12] =
{
	float3(0.0f, 0.5f, 0.0f),
	float3(0.5f, 1.0f, 0.0f),
	float3(1.0f, 0.5f, 0.0f),
	float3(0.5f, 0.0f, 0.0f),
	float3(0.0f, 0.5f, 1.0f),
	float3(0.5f, 1.0f, 1.0f),
	float3(1.0f, 0.5f, 1.0f),
	float3(0.5f, 0.0f, 1.0f),
	float3(0.0f, 0.0f, 0.5f),
	float3(0.0f, 1.0f, 0.5f),
	float3(1.0f, 1.0f, 0.5f),
	float3(1.0f, 0.0f, 0.5f)
};

static const int TerrainVoxelIdxLookUpTable[6][6] = 
{
	{ 0, 0, 0, 0, 0, 0 },
	{ 0, 8, 3, 0, 0, 0 },
	{ 0, 1, 9, 0, 0, 0 },
	{ 1, 8, 3, 9, 8, 1 },
	{ 1, 2, 10, 0, 0, 0 },
	{ 0, 8, 3, 1, 2, 10 }
};

cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
	float4   gColor;
};

struct VertexIn
{
	float3 PositionIdx : POSITION;
	float4x2 Density : TEXCOORD;
	float3x4 FNormal  : FNORMAL;
	float3x4 BNormal  : BNORMAL;
	uint InstanceId : SV_InstanceID;
};

struct GeoOut
{
	float4 PosH		: SV_POSITION;
	float3 PosW		: POSITION;
	float3 Normal   : NORMAL;
	uint   PrimeID	: SV_PrimitiveID;
};

//Texture2DArray gTreeTexArray;
Texture2DArray gDiffuseMap;
SamplerState DefaultSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};


VertexIn VS(VertexIn IN)
{
	return IN;
}

[maxvertexcount(8)]
void GS(point VertexIn gin[1], uint primeID : SV_PrimitiveID, inout TriangleStream<GeoOut> triStream)
{
	int index = 5;
	//float3 right = float3(4, 0, 0);
	//float3 up = float3(0, 4, 0);

	//float4 v[4];
	//v[0] = float4(gin[0].PositionIdx.xyz + right - up, 1.0f);
	//v[1] = float4(gin[0].PositionIdx.xyz + right + up, 1.0f);
	//v[2] = float4(gin[0].PositionIdx.xyz - right - up, 1.0f);
	//v[3] = float4(gin[0].PositionIdx.xyz - right + up, 1.0f);

	//GeoOut gout;
	//[unroll]
	//for (int i = 0; i < 4; ++i)
	//{
	//	gout.PosH = mul(v[i], gWorldViewProj);
	//	gout.PosW = v[i].xyz;
	//	gout.Normal = float3(1,0,0);
	//	gout.PrimeID = primeID;

	//	triStream.Append(gout);
	//}
	float3 test = float3(gin[0].BNormal[0].xy, 0);
	GeoOut gout;
	[unroll]
	for (int i = 0; i < 6; ++i)
	{
		int idx = TerrainVoxelIdxLookUpTable[index][i];
		gout.PosW = gin[0].PositionIdx.xyz + TerrainVoxelVertexLookUpTable[idx];
		gout.PosH = mul(float4(gout.PosW.xyz, 1.0f), gWorldViewProj);
		gout.Normal = test;
		gout.PrimeID = primeID;
		triStream.Append(gout);
		++i;

		idx = TerrainVoxelIdxLookUpTable[index][i];
		gout.PosW = gin[0].PositionIdx.xyz + TerrainVoxelVertexLookUpTable[idx];
		gout.PosH = mul(float4(gout.PosW.xyz, 1.0f), gWorldViewProj);
		gout.Normal = test;
		gout.PrimeID = primeID;
		triStream.Append(gout);
		++i;

		idx = TerrainVoxelIdxLookUpTable[index][i];
		gout.PosW = gin[0].PositionIdx.xyz + TerrainVoxelVertexLookUpTable[idx];
		gout.PosH = mul(float4(gout.PosW.xyz, 1.0f), gWorldViewProj);
		gout.Normal = test;
		gout.PrimeID = primeID;
		triStream.Append(gout);

		triStream.RestartStrip();
	}
}

float4 PS(GeoOut IN) : SV_Target
{
	return float4(IN.Normal.xyz,1);
}

technique11 TerrainTech
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(CompileShader(gs_5_0, GS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}
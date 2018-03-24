#include "DefaultLightFunc.fx"

static float3 up = { 0.0f, 1.0f, 0.0f };

cbuffer cbPerFrame
{
	Light gDirLight;
	PointLight gPointLight;
	SpotLight gSpotLight;
	float3 gEyePosW;
};

cbuffer cbPerBillboard
{
	float4x4 gViewProj;
	Material gMaterial;
};

cbuffer cbFixed
{
	float2 gTexC[4] =
	{
		float2(0.0f, 1.0f),
		float2(0.0f, 0.0f),
		float2(1.0f, 1.0f),
		float2(1.0f, 0.0f)
	};
}

struct VertexIn
{
	float3 Pos : POSITION;
	float2 Size : SIZE;
};

struct GeoOut
{
	float4 PosH		: SV_POSITION;
	float3 PosW		: POSITION;
	float3 Normal  : NORMAL;
	float2 Tex		: TEXCOORD;
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
	VertexIn OUT;

	OUT.Pos = IN.Pos;
	OUT.Size = IN.Size;

	return OUT;
}

[maxvertexcount(4)]
void GS(point VertexIn gin[1], uint primeID : SV_PrimitiveID, inout TriangleStream<GeoOut> triStream)
{
	float3 look = gEyePosW - gin[0].Pos;
	look.y = 0.0f;
	look = normalize(look);
	float3 right = cross(up, look);

	float4 v[4];
	v[0] = float4(gin[0].Pos + gin[0].Size.x * right - gin[0].Size.y * up, 1.0f);
	v[1] = float4(gin[0].Pos + gin[0].Size.x * right + gin[0].Size.y * up, 1.0f);
	v[2] = float4(gin[0].Pos - gin[0].Size.x * right - gin[0].Size.y * up, 1.0f);
	v[3] = float4(gin[0].Pos - gin[0].Size.x * right + gin[0].Size.y * up, 1.0f);

	GeoOut gout;
	[unroll]
	for (int i = 0; i < 4; ++i)
	{
		gout.PosH = mul(v[i], gViewProj);
		gout.PosW = v[i].xyz;
		gout.Normal = look;
		gout.Tex = gTexC[i];
		gout.PrimeID = primeID;

		triStream.Append(gout);
	}
}

float4 PS(GeoOut IN) : SV_Target
{
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
	float4 texColor = gDiffuseMap.Sample(DefaultSampler, uvw);

	float4 litColor = texColor * (ambient + diffuse) + specular;
	litColor.a = texColor.a;

	//clip(texColor.a - 0.1f);

	return texColor;
}

technique11 LightBillboardTech
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(CompileShader(gs_5_0, GS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}
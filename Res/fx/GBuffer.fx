#include "Utill.fx"

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldViewProj;
};

struct Vertex_In
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float3 BiNormal : BINORMAL;
	float2 Tex : TEXCOORD;
	dword  TexID : TEXID;
};

struct Vertex_Out
{
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float3 BiNormal : BINORMAL;
	float2 Tex : TEXCOORD;
	dword  TexID : TEXID;
};

struct PS_GBUFFER_OUT
{
	float4 ColorSpecInt : SV_TARGET0;
	float4 Normal : SV_TARGET1;
	float4 SpecPow : SV_TARGET2;
};

Texture2DArray gDiffuseMap;
Texture2DArray gNormalMap;
SamplerState DefaultSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	//MaxAnisotropy = 4;
	AddressU = Wrap;
	AddressV = Wrap;
};

PS_GBUFFER_OUT PackGBufferFunc(float3 BaseColor, float3 Normal, float SpecIntensity, float SpecPower)
{
	PS_GBUFFER_OUT Out;

	// Normalize the specular power
	//float SpecPowerNorm = max(0.0001, (SpecPower - g_SpecPowerRange.x) / g_SpecPowerRange.y);

	// Pack all the data into the GBuffer structure
	Out.ColorSpecInt = float4(BaseColor.rgb, SpecIntensity);
	Out.Normal = float4(Normal * 0.5 + 0.5, 0.0);
	//Out.SpecPow = float4(SpecPowerNorm, 0.0, 0.0, 0.0);
	Out.SpecPow = float4(0.0f, 0.0, 0.0, 0.0);

	return Out;
}

Vertex_Out PackGBuffer_VS(Vertex_In IN)
{
	Vertex_Out OUT;

	OUT.PosW = mul(float4(IN.Pos, 1.0f), gWorld).xyz;
	OUT.Normal = mul(IN.Normal, (float3x3)gWorld);
	OUT.Tangent = mul(IN.Tangent, (float3x3)gWorld);
	OUT.BiNormal = mul(IN.BiNormal, (float3x3)gWorld);
	OUT.PosH = mul(float4(IN.Pos, 1.0f), gWorldViewProj);

	OUT.Tex = IN.Tex;
	OUT.TexID = IN.TexID;

	return OUT;
}

PS_GBUFFER_OUT PackGBuffer_PS(Vertex_Out IN)
{
	float3 uvw = float3(IN.Tex, IN.TexID);

	float3 normal = IN.Normal;
	ClacNormalMap(gNormalMap.Sample(DefaultSampler, uvw).xyz, IN.Tangent, IN.BiNormal, IN.Normal, normal);

	return PackGBufferFunc(gDiffuseMap.Sample(DefaultSampler, uvw).xyz, normal, 1.f, 1.f);
}

technique11 PackGBufferTech
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, PackGBuffer_VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PackGBuffer_PS()));
	}
}

//==============================================================================================================




static const float2 arrOffsets[4] = {
	float2(-0.75, -0.75),
	float2(-0.25, -0.75),
	float2(0.25, -0.75),
	float2(0.75, -0.75),
};

static const float2 arrBasePos[4] = {
	float2(1.0, 1.0),
	float2(1.0, -1.0),
	float2(-1.0, 1.0),
	float2(-1.0, -1.0),
};

static const float2 arrUV[4] = {
	float2(1.0, 0.0),
	float2(1.0, 1.0),
	float2(0.0, 0.0),
	float2(0.0, 1.0),
};

static const float4 arrMask[4] = {
	float4(1.0, 0.0, 0.0, 0.0),
	float4(0.0, 1.0, 0.0, 0.0),
	float4(0.0, 0.0, 1.0, 0.0),
	float4(0.0, 0.0, 0.0, 1.0),
};

struct PACK_DATA
{
	float LinearDepth;
	float3 Color;
	float3 Normal;
	float SpecPow;
	float SpecIntensity;
};

cbuffer cbGBufferUnpack : register(b0)
{
	float4   PerspectiveValues : packoffset(c0);
	float4x4 ViewInv           : packoffset(c1);
}

struct VS_OUTPUT
{
	float4 Position	: SV_Position; // vertex position 
	float2 TexUV	: TEXCOORD0;   // vertex texture coords
	float4 sampMask	: TEXCOORD1;
};

Texture2D gDepthTex			: register(t0);
Texture2D gColorTex			: register(t1);
Texture2D gNormalTex        : register(t2);
Texture2D gSpecTex			: register(t3);
SamplerState GBufferSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	//MaxAnisotropy = 4;
};

PACK_DATA UnpackGBufferFunc(Texture2D ColorTex, Texture2D NormalTex, Texture2D SpecTex, Texture2D DepthTex, float2 UV)
{
	PACK_DATA Out;

	float depth = DepthTex.Sample(GBufferSampler, UV.xy).x;
	Out.LinearDepth = ConvertZToLinearDepth(depth, PerspectiveValues);

	float4 baseColorSpecInt = ColorTex.Sample(GBufferSampler, UV.xy);
	Out.Color = baseColorSpecInt.xyz;
	Out.SpecIntensity = baseColorSpecInt.w;

	Out.Normal = NormalTex.Sample(GBufferSampler, UV.xy).xyz;
	Out.Normal = normalize(Out.Normal * 2.0 - 1.0);

	Out.SpecPow = SpecTex.Sample(GBufferSampler, UV.xy).x;

	return Out;
}

VS_OUTPUT UnPackGBuffer_VS(uint VertexID : SV_VertexID)
{
	VS_OUTPUT Output;

	Output.Position = float4(arrBasePos[VertexID].xy, 0.0, 1.0);
	Output.TexUV = arrUV[VertexID].xy;
	Output.sampMask = arrMask[VertexID].xyzw;

	return Output;
}

float4 UnPackGBuffer_PS(VS_OUTPUT In) : SV_TARGET
{
	PACK_DATA data = UnpackGBufferFunc(gColorTex, gNormalTex, gSpecTex, gDepthTex, In.TexUV);

	return float4(data.LinearDepth.x, 0, 0, 1.0f);
}

technique11 UnPackGBufferTech
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, UnPackGBuffer_VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, UnPackGBuffer_PS()));
	}
}
#include "Utill.fxh"

struct PS_GBUFFER_OUT
{
	float4 ColorSpecInt : SV_TARGET0;
	float4 Normal : SV_TARGET1;
	float4 SpecPow : SV_TARGET2;
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

cbuffer cbGBufferUnpack : register(b0)
{
	float4   PerspectiveValues : packoffset(c0);
	float4x4 ViewInv           : packoffset(c1);
}

struct PACK_DATA
{
	float LinearDepth;
	float3 Color;
	float3 Normal;
	float SpecPow;
	float SpecIntensity;
};

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
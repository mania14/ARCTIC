#include "Utill.fxh"

struct VertexIn
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD;
};

struct VertexOut
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
};

Texture2D gDiffuseMap;
SamplerState DefaultSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	//MaxAnisotropy = 4;
};


VertexOut VS(VertexIn IN)
{
	VertexOut OUT;

	OUT.Pos = float4(IN.Pos, 1);
	OUT.Tex = IN.Tex;

	return OUT;
}

float4 PS(VertexOut IN) : SV_Target
{
	float v = 0.3 + 0.7* PerlinNoise2D(IN.Tex.x, IN.Tex.y);

	return float4(v,v,v,1);
}

technique11 PerlinNoise
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}
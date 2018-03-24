cbuffer cbPerFrame
{
	float4x4 gWorldViewProj;
};

TextureCube gCubeMap;

SamplerState SamperCubeMap
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VertexIn
{
	float3 PosL : POSITION;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float2 Tex : TEXCOORD;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float3 PosL : POSITION;
};

VertexOut VS(VertexIn vIn)
{
	VertexOut vout;
	vout.PosH = mul(float4(vIn.PosL, 1.0f), gWorldViewProj).xyww;

	vout.PosL = vIn.PosL;

	return vout;
}

float4 PS(VertexOut pIn) : SV_Target
{
	return gCubeMap.Sample(SamperCubeMap, pIn.PosL);	
}

RasterizerState NoCull
{
	CullMode = None;
};

DepthStencilState LessEqualDSS
{
	DepthFunc = LESS_EQUAL;
};

technique11 SkyTech
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));

		SetRasterizerState(NoCull);
		SetDepthStencilState(LessEqualDSS, 0);
	}
}
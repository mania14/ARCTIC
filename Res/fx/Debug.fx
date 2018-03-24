cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
	float4   gColor;
};

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
};

struct VertexIn
{
	float3 Pos : POSITION;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VertexIn IN)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(float4(IN.Pos, 1.0f), gWorldViewProj);
	return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
	return gColor;
}

technique11 DebugTech
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}
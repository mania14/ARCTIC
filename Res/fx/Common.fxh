BlendState NoBlend
{
	AlphaToCoverageEnable = FALSE;
	BlendEnable[0] = FALSE;
};

RasterizerState CCW
{
	CullMode = BACK;
};

RasterizerState CW
{
	CullMode = FRONT;
};

DepthStencilState UseDepth
{
	DepthEnable = TRUE;
	StencilEnable = FALSE;
	DepthWriteMask = ALL;
};

DepthStencilState NoUseDepth
{
	DepthEnable = FALSE;
	StencilEnable = FALSE;
	DepthWriteMask = 0;
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

struct ScreenSquadVS_Output
{
	float4 Position	: SV_Position; // vertex position 
	float2 UV		: TEXCOORD0;   // vertex texture coords
};

ScreenSquadVS_Output ScreenSquadVS(uint VertexID : SV_VertexID)
{
	ScreenSquadVS_Output Output;

	Output.Position = float4(arrBasePos[VertexID].xy, 0.0, 1.0);
	Output.UV = arrUV[VertexID].xy;

	return Output;
}

Texture2D DepthTexture      : register(t0);
Texture2D gColorTex			: register(t1);
Texture2D gNormalTex        : register(t2);
Texture2D gSpecTex			: register(t3);

SamplerState DefaultSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	//MaxAnisotropy = 4;
};

static float2 arrOffsets[4] = {
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

struct VS_OUTPUT
{
	float4 Position	: SV_Position; // vertex position 
	float2 UV		: TEXCOORD0;   // vertex texture coords
	float4 sampMask	: TEXCOORD1;
};

VS_OUTPUT ScreenSquadVS(uint VertexID : SV_VertexID)
{
	VS_OUTPUT Output;

	Output.Position = float4(arrBasePos[VertexID].xy, 0.0, 1.0);
	Output.UV = arrUV[VertexID].xy;
	Output.sampMask = arrMask[VertexID].xyzw;

	return Output;
}

float4 ScreenSquadPS(VS_OUTPUT In) : SV_TARGET
{
	//SURFACE_DATA gbd = UnpackGBuffer(In.UV.xy);
	float4 finalColor = float4(1.0, 0.0, 0.0, 1.0);
	finalColor = gColorTex.Sample(DefaultSampler, In.UV);
	//finalColor += float4(1.0 - saturate(gbd.LinearDepth / 75.0), 1.0 - saturate(gbd.LinearDepth / 125.0), 1.0 - saturate(gbd.LinearDepth / 200.0), 0.0) * In.sampMask.xxxx;
	//finalColor += float4(gbd.Color.xyz, 0.0) * In.sampMask.yyyy;
	//finalColor += float4(gbd.Normal.xyz * 0.5 + 0.5, 0.0) * In.sampMask.zzzz;
	//finalColor += float4(gbd.SpecIntensity, gbd.SpecPow, 0.0, 0.0) * In.sampMask.wwww;

	return finalColor;
}

technique11 ScreenSquadTech
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, ScreenSquadVS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, ScreenSquadPS()));
	}
}
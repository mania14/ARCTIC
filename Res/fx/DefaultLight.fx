#include "DefaultLightFunc.fx"
#include "GBuffer.fx"

cbuffer cbPerFrame
{
	Light gDirLight;
	float3 gEyePosW;
};

float4 LightDefault_PS(VS_OUTPUT In) : SV_Target
{
	PACK_DATA Gdata = UnpackGBufferFunc(gColorTex, gNormalTex, gSpecTex, gDepthTex, In.TexUV);

	//���ʿ� ��ī�̹ڽ� �ȼ��� ����������� ���߿� �ȼ����̴����� ������������ (�� �ٵ� �켱 �������� �������ؾ���)
	float depth = gDepthTex.Sample(GBufferSampler, In.TexUV).x;
	if (depth == 1.f)
	{
		return float4(Gdata.Color, 1.f);
	}

	float4 A, D, S;
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	Material gMaterial;
	gMaterial.Ambient = ambient;
	gMaterial.Diffuse = float4(Gdata.Color, 1.f);
	gMaterial.Specular = specular;

	float3 worldPos = CalcWorldPos(In.Position.xy, Gdata.LinearDepth, ViewInv, PerspectiveValues);
	float3 toEyeW = normalize(gEyePosW - worldPos);

	CalcDirectionalLight(gMaterial, gDirLight, Gdata.Normal, toEyeW, A, D, S);

	ambient += A;
	diffuse += D;
	specular += S;

	float4 color = diffuse + specular + ambient;
	color.a = 1;

	return color;

	//CalcPointLight(gMaterial, gPointLight, IN.PosW, IN.Normal, toEyeW, A, D, S);
	//ambient += A;
	//diffuse += D;
	//specular += S;

	// ����� �ȹݻ縦 ���ؼ� �Ÿ������ �ʿ��ϴ�
	/*float3 reflectVector = reflect(-toEyeW, IN.Normal);
	reflectVector = lerp(reflectVector, IN.Normal, 0.3f);
	float4 GIColor = gCubeMap.Sample(SamperCubeMap, reflectVector);*/
}

technique11 LightDefaultTech
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, UnPackGBuffer_VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, LightDefault_PS()));
	}
}
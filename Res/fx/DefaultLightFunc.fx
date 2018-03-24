struct Light
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float  Range;
	float3 Attenuation;
	float  Spot;
	float3 Position;
	float  Dummy;
};

struct PointLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Attenuation;
	float Dummy;
};

struct SpotLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Direction;
	float Spot;

	float3 Att;
	float Dummy;
};

struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular; // w = SpecPower
	float4 Reflect;
};

void CalcDirectionalLight( Material mat, Light L, float3 normal, float3 toEye,
							out float4 ambient, out float4 diffuse, out float4 spec)
{
	float3 lightDir = -L.Direction;

	ambient = mat.Ambient * L.Ambient;

	float diffuseFactor = dot(lightDir, normal);

	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightDir, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);

		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec = specFactor * mat.Specular * L.Specular;
	}
}

void CalcPointLight( Material mat, Light L, float3 pos, float3 normal, float3 toEye,
					out float4 ambient, out float4 diffuse, out float4 spec)
{
	float3 lightDir = -L.Position - pos;

	float d = length(lightDir);

	if (d > L.Range)
		return;

	// ºû º¤ÅÍÀÇ Á¤±ÔÈ­
	lightDir /= d;

	ambient = mat.Ambient * L.Ambient;

	float diffuseFactor = dot(lightDir, normal);

	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightDir, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);

		diffuse = diffuseFactor * mat.Diffuse, L.Diffuse;
		spec = specFactor * mat.Specular * L.Specular;
	}

	//°¨¼è
	float att = 1.0f / dot(L.Attenuation, float3(1.0f, d, d*d));

	diffuse *= att;
	spec *= att;
}
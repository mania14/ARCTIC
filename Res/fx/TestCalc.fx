
struct Data
{
	float3 v1;
	float2 v2;
};

StructuredBuffer<Data> gInputA;
StructuredBuffer<Data> gInputB;
RWStructuredBuffer<Data> gOutput;

//Texture2D gInputA;
//Texture2D gInputB;
//RWTexture2D<float4> g_Output;

cbuffer cbSettings
{
	
};

//[numthreads(16, 16, 1)]
//void CS(int3 dispatchThreadID : SV_DispatchThreadID)
//{
//	gOutput[dispatchThreadID.xy] = gInputA[dispatchThreadID.xy] + gInputB[dispatchThreadID.xy];
//}

[numthreads(32, 1, 1)]
void CS(int3 dispatchThreadID : SV_DispatchThreadID)
{
	gOutput[dispatchThreadID.x].v1 = gInputA[dispatchThreadID.x].v1 + gInputB[dispatchThreadID.x].v1;
	gOutput[dispatchThreadID.x].v2 = gInputA[dispatchThreadID.x].v2 + gInputB[dispatchThreadID.x].v2;
}

technique11 AddTextures
{
	pass p0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS()));
	}
}
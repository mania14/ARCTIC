cbuffer cbSettings
{
	float gWeights[11] =
	{
		0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f
	};
}

cbuffer cbFixed
{
	static const int gBlurRadious = 5;
}

Texture2D gInput;
RWTexture2D<float4> gOutput;

#define N 256
#define CacheSize (N + 2*gBlurRadious)
groupshared float4 gCache[CacheSize];

[numthreads(N, 1, 1)]

void HorizonBlurCS(int3 groupThreadID : Sv_GroupThreadID, int3 dispatchThreadID : SV_DispatchThreadID)
{
	if (groupThreadID.x < gBlurRadious)
	{
		int x = max(dispatchThreadID.x - gBlurRadious, 0);
		gCache[groupThreadID.x] = gInput[int2(x, dispatchThreadID.y)];
	}

	if (groupThreadID.x >= N - gBlurRadious)
	{
		int x = min(dispatchThreadID.x + gBlurRadious, gInput.Length.x - 1);						// 텍스쳐의 한줄 길이 gInput.Length.x-1
		gCache[groupThreadID.x + 2 * gBlurRadious] = gInput[int2(x, dispatchThreadID.y)];
	}

	gCache[groupThreadID.x + gBlurRadious] = gInput[min(dispatchThreadID.xy, gInput.Length.xy - 1)];

	// 모든 스레드가 여기까지 작업을 완수할때까지 기다린다.
	GroupMemoryBarrierWithGroupSync();

	// 본격적인 흐리기기기기리리리!!
	float4 blurColor = float4(0, 0, 0, 0);

	[unroll]
	for (int i = -gBlurRadious; i <= gBlurRadious; ++i)
	{
		int k = groupThreadID.x + gBlurRadious + i;
		blurColor += gWeights[i + gBlurRadious] * gCache[k];
	}

	gOutput[dispatchThreadID.xy] = blurColor;
}

[numthreads(1, N, 1)]
void VertBlurCS(int3 groupThreadID : SV_GroupThreadID, int3 dispatchThreadID : SV_DispatchthreadID)
{
	if (groupThreadID.y < gBlurRadious)
	{
		int y = max(dispatchThreadID.y - gBlurRadious, 0);
		gCache[groupThreadID.y] = gInput[int2(dispatchThreadID.x, y)];
	}

	if (groupThreadID.y >= N - gBlurRadious)
	{
		int y = min(dispatchThreadID.y + gBlurRadious, gInput.Length.y - 1);
		gCache[groupThreadID.y + 2 * gBlurRadious] = gInput[int2(dispatchThreadID.x, y)];
	}

	// 모든 스레드가 여기까지 작업을 완수할때까지 기다린다.
	GroupMemoryBarrierWithGroupSync();

	// 본격적인 흐리기기기기리리리!!
	float4 blurColor = float4(0, 0, 0, 0);

	[unroll]
	for (int i = -gBlurRadious; i <= gBlurRadious; ++i)
	{
		int k = groupThreadID.y + gBlurRadious + i;
		blurColor += gWeights[i + gBlurRadious] * gCache[k];
	}

	gOutput[dispatchThreadID.xy] = blurColor;
}

technique11 HorizonBlur
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, HorizonBlurCS()));
	}
}

technique11 VerticalBlur
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, VertBlurCS()));
	}
}
BlendState NoBlend
{
	AlphaToCoverageEnable = FALSE;
	BlendEnable[0] = FALSE;
};

RasterizerState CCW
{
	CullMode = BACK;
};

DepthStencilState UseDepth
{
	DepthEnable = TRUE;
	StencilEnable = FALSE;
	DepthWriteMask = ALL;
};
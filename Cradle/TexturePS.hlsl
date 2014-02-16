Texture2D colorMap_ : register(t0);
SamplerState colorSampler_ : register(s0);

struct PS_Input
{
	float4 pos : SV_POSITION;
	float2 tex0 : TEXCOORD0;
};


float4 PSMain(PS_Input frag) : SV_TARGET
{
	return colorMap_.Sample(colorSampler_, frag.tex0);
}
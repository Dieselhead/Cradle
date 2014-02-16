cbuffer cbTransform
{
	float4x4 WVP;
}

float4 VSMain( float4 pos : POSITION ) : SV_POSITION
{
	pos = mul(pos, WVP);
	return pos;
}
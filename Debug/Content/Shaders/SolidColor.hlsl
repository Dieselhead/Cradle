cbuffer cbTransform
{
	float4x4 WVP;
	float4x4 View;
	float4x4 Projection;
	float4x4 ViewProj;
};

struct VSInput
{
	float4 pos : POSITION;
	float3 normal : NORMAL;
	float2 tex0 : TEXCOORD0;
	float4x4 w0 : OBJ0;
	// float4 w1 : OBJ1;
	// float4 w2 : OBJ2;
	// float4 w3 : OBJ3;
	uint id : Sv_InstanceID;
};

struct PSInput
{
	float4 p : SV_POSITION;
	float3 c : NORMAL;
	float2 tex0 : TEXCOORD0;
};



// PSInput VSMain(float4 pos : POSITION, float4 w0 : OBJ0, float4 w1 : OBJ1, float4 w2 : OBJ2, float4 w3 : OBJ3)
PSInput VSMain(VSInput foo)
{
	PSInput psin = (PSInput)0;



	foo.pos.w = 1.0f;


	float4 op = foo.pos;

	// op.x += (floor(foo.id / 50) * 5);
	// op.y += (foo.id % 50) * 5;

	float4x4 mx;
	

	mx = foo.w0;
	
	

	float4x4 bar = mul(mx, ViewProj);

	op = mul(op, bar);

	float3 nom;

	nom.xyz = foo.normal.xyz;
	// nom.w = 1.0f;




	// float4x4 viewProj = mul(View, Projection);

	
	

	// psin.c = float3(inst.x, inst.y, inst.z);
	// psin.c = foo.w0;

	// psin.c = float4(w0.x, w0.y, w0.z, w0.w);


	psin.p = op;
	psin.c = mul(nom, mx);
	// psin.c = nom;

	psin.tex0 = foo.tex0;

	return psin;
}

float4 PSMain(PSInput psin) : SV_TARGET
{

	// psin.c = normalize(psin.c);

	psin.c.x = (psin.c.x + 1.0f) * 0.5f;
	psin.c.y = (psin.c.y + 1.0f) * 0.5f;
	psin.c.z = (psin.c.z + 1.0f) * 0.5f;

	// float r = dot(float3(1, 0, 0), psin.c.x);
	// float g = dot(float3(0, 1, 0), psin.c.y);
	// float b = dot(float3(0, 0, 1), psin.c.z);


	// Texcoords
	// return float4(psin.tex0.xy, 0.0f, 1.0f);

	// Normals
	return float4(psin.c.xyz, 1.0f);


	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
#include "VertexHelper.txt"
#include "LightHelper.txt"
#include "SamplerStates.txt"

Texture2D gNormalDiffuseMap : register ( t0 );

struct PatchTess
{
	float EdgeTess[3]   : SV_TessFactor;
	float InsideTess	: SV_InsideTessFactor;
};

//Commented structures because of #include
/*struct DomainOut
{
	float4 PosH : SV_POSITION;
};

struct HullOut
{
	float4 PosL : SV_POSITION;
};*/

cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}

cbuffer cbPerFrame : register(b1)
{
	DirectionalLight gDirLight;
	PointLight gPointLight;
	SpotLight gSpotLight;
	float3 gEyePosW;
};

cbuffer cbPerObject : register(b2)
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	float4x4 gViewProj;
	float4x4 gWorldViewProj;
	float4x4 gTexTransform;
	Material gMaterial;
};


// The domain shader is called for every vertex created by the tessellator.  
// It is like the vertex shader after tessellation.
[domain("tri")]
DomainOut DS(PatchTess patchTess,
	float3 bary : SV_DomainLocation,
	const OutputPatch<HullOut, 3> tri)
{
	DomainOut dout;

	//Commented matrix mult is ToshIP
	//matrix gWorldViewProj = mul(World, View);
	//gWorldViewProj = mul(gWorldViewProj, Projection);

	// Bilinear interpolation.
	/*float4 v1 = lerp(quad[0].PosL, quad[1].PosL, uv.x);
		float4 v2 = lerp(quad[0].PosL, quad[2].PosL, uv.y);
		float4 p = v1 + v2;*/
		// Displacement mapping
		//p.y = 0.3f*(p.z*sin(p.x) + p.x*cos(p.z));
		//w = 1.0f;

	//float4 p = bary.x * tri[0].PosL + bary.y * tri[1].PosL + bary.z * tri[2].PosL;
		//p.w = 1;
	//p.y = 0.3f*(p.z*sin(p.x) + p.x*cos(p.z));
	//dout.PosH = mul(p, gWorldViewProj);

	// Transform to world space space.
	//dout.PosW = mul(p, gWorld).xyz;
	//dout.NormalW = mul(.NormalL, (float3x3)gWorldInvTranspose);

	// Interpolate patch attributes to generated vertices.
	dout.PosW = bary.x*tri[0].PosW + bary.y*tri[1].PosW + bary.z*tri[2].PosW;
	dout.NormalW = bary.x*tri[0].NormalW + bary.y*tri[1].NormalW + bary.z*tri[2].NormalW;
	dout.PosL = bary.x*tri[0].PosL + bary.y*tri[1].PosL + bary.z*tri[2].PosL;
	//Commented because no Tangent currently in the implementation
	//dout.TangentW = bary.x*tri[0].TangentW + bary.y*tri[1].TangentW + bary.z*tri[2].TangentW;
	

	// Choose the mipmap level based on distance to the eye; specifically, choose
	// the next miplevel every MipInterval units, and clamp the miplevel in [0,6].
	//const float MipInterval = 20.0f;
	//float mipLevel = clamp((distance(dout.PosW, gEyePosW) - MipInterval) / MipInterval, 0.0f, 6.0f);

	
	dout.UV = bary.x*tri[0].UV + bary.y*tri[1].UV + bary.z*tri[2].UV;
	dout.DisplacementTex = bary.x*tri[0].DisplacementTex + bary.y*tri[1].DisplacementTex + bary.z*tri[2].DisplacementTex;


	// Interpolating normal can unnormalize it, so normalize it.
	dout.NormalW = normalize(dout.NormalW);

	// Sample height map (stored in alpha channel).
	float h = gNormalDiffuseMap.SampleLevel(samLinear, dout.DisplacementTex, 1).a;

	// Offset vertex along normal.
	//dout.PosW += h*dout.NormalW*0.2f;

	// Transform to homogeneous clip space.
	// Project to homogeneous clip space.
	//dout.PosH = mul(float4(dout.PosW, 1.0f), gViewProj);
	dout.PosH = dout.PosL;

	return dout;
}

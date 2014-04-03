#include "VertexHelper.txt"
#include "LightHelper.txt"

struct PatchTess
{
	float EdgeTess[3]   : SV_TessFactor;
	float InsideTess : SV_InsideTessFactor;
};

cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}


PatchTess ConstantHS(InputPatch<VertexOut, 3> patch, uint patchID : SV_PrimitiveID)
{
	PatchTess pt;

	matrix gWorld = World;

	//Commented centerL and centerW because "invalid subscript 'PosL'";
	//float3 centerL = (patch[0].PosL + patch[1].PosL + patch[2].PosL) / (3.00f);
		//float3 centerW = mul(float4(centerL, 1.0f), gWorld).xyz;

		//float d = distance(centerW, gEyePosW);

		// Tessellate the patch based on distance from the eye such that
		// the tessellation is 0 if d >= d1 and 60 if d <= d0.  The interval
		// [d0, d1] defines the range we tessellate in.

		//const float d0 = 20.0f;
		//const float d1 = 100.0f;
		//float tess = 64.0f*saturate((d1 - d) / (d1 - d0));

		float tess = 2;

	// Uniformly tessellate the patch.

	pt.EdgeTess[0] = tess;
	pt.EdgeTess[1] = tess;
	pt.EdgeTess[2] = tess;
	//pt.EdgeTess[3] = tess;

	pt.InsideTess = 5;

	return pt;
}


[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
HullOut HS(InputPatch<VertexOut, 3> p,
	uint i : SV_OutputControlPointID,
	uint patchId : SV_PrimitiveID)
{
	HullOut hout;

	// Pass through shader.
	hout.PosW = p[i].PosW;
	hout.PosL = p[i].PosL;
	hout.NormalW = p[i].NormalW;
	hout.PosH = p[i].PosH;
	hout.NormalL = p[i].NormalL; 
	hout.UV = p[i].UV;
	hout.DisplacementTex = p[i].DisplacementTex;
	hout.TangentL = p[i].TangentL;
	
	//Commented because no Tangent currently
	//hout.TangentW = p[i].TangentW;
	
	
	



	return hout;
}




/*struct VertexIn
{
	float4 PosL    : POSITION;
};

struct VertexOut
{
	float4 PosL    : SV_POSITION;
};

struct HullOut
{
	float4 PosL : SV_POSITION;
};*/
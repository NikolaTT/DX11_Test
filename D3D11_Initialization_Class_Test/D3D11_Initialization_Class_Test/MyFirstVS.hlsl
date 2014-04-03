#include "VertexHelper.txt"
#include "LightHelper.txt"
#include "SamplerStates.txt"

/*struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float pad;
};

struct PointLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Att;
	float pad;
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
	float pad;
};

struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular; // w = SpecPower
	float4 Reflect;
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

/*struct VertexIn
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
	float2 UV : TEXTURE;
};

struct VertexOut
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : POSITION;
	float3 NormalW : NORMAL;
	float2 UV : TEXTURE;
};*/


/*struct VertexIn
{
	float4 PosL    : POSITION;
};

struct VertexOut
{
	float4 PosL    : SV_POSITION;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	vout.PosL = vin.PosL;

	return vout;
}*/

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	vout.PosL = vin.PosL;
	// Transform to world space space.
	vout.PosW = vin.PosL;
	//vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);
	vout.PosH = float4(0, 0, 0, 0);
	vout.NormalL = vin.NormalL;
	vout.UV = vin.UV;
	vout.DisplacementTex = vin.DisplacementTex;
	vout.TangentL = vin.TangentL;
	
	
	//Commented because no Tangent currently
	//vout.TangentW = mul(vin.TangentL, (float3x3)gWorld);


	// Transform to world space space.
	//vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	//vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);

	// Transform to homogeneous clip space.
	//vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	//vout.PosH = float4(vin.PosL, 1.0f);

	return vout;
}










/*
VertexOut VS(VertexIn vIn)
{
	//Pos = mul(Pos, World);
	//Pos = mul(Pos, View);
	//Pos = mul(Pos, Projection);
	return Pos;
}*/

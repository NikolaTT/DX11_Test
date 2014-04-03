#include "VertexHelper.txt"
#include "LightHelper.txt"
#include "SamplerStates.txt"

/*cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}*/

TextureCube gCubeMap : register(t0);
Texture2D gCubeSideTexture : register(t1);

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

float4 PS(DomainOut pin) : SV_Target
{

	pin.PosH.z = 1.0f;
	// Interpolating normal can unnormalize it, so normalize it.
	pin.NormalW = normalize(pin.NormalW);

	float3 toEyeW = normalize(gEyePosW - pin.PosW);

		// Start with a sum of zero. 
		float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

		// Sum the light contribution from each light source.
		float4 A, D, S;

	ComputeDirectionalLight(gMaterial, gDirLight, pin.NormalW, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;

	ComputePointLight(gMaterial, gPointLight, pin.PosW, pin.NormalW, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;

	ComputeSpotLight(gMaterial, gSpotLight, pin.PosW, pin.NormalW, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;

	float4 litColor = ambient + diffuse + spec;

		// Common to take alpha from diffuse material.
	litColor.a = gMaterial.Diffuse.a;

	return litColor;

	//return litColor * gCubeSideTexture.Sample(samAnisotropic, pin.UV);
	//return litColor + gCubeMap.Sample(samTriLinearSam, pin.PosL);

	//return gCubeMap.Sample(samTriLinearSam, pin.PosL);
	//return float4(1.0f, 0.0f, 0.0f, 1.0f);
}

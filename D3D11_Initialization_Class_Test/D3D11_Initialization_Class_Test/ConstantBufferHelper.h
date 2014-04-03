#ifndef H_CONSTANTBUFFER
#define H_CONSTANTBUFFER


#include "LightTypes.h"

struct cbPerFrame{
	DirectionalLight gDirLight;
	PointLight gPointLight;
	SpotLight gSpotLight;
	DirectX::XMFLOAT3 gEyePosW;
};

struct cbPerObject{
	DirectX::XMMATRIX gWorld;
	DirectX::XMMATRIX gWorldInvTranspose;
	DirectX::XMMATRIX gViewProj;
	DirectX::XMMATRIX gWorldViewProj;
	DirectX::XMMATRIX gTexTransform;
	Material gMaterial;

	/*float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	float4x4 gViewProj;
	float4x4 gWorldViewProj;
	float4x4 gTexTransform;
	Material gMaterial;*/
};

#endif
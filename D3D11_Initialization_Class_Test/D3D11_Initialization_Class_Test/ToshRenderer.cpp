#include "ToshRenderer.h"
#include <DirectXMath.h>
using namespace DirectX;

ToshRenderer::ToshRenderer(D3DInitializer* mTDevice){// : mDirLight(), mPointLight(), mSpotLight(), CubeMat(){
	
	this->mTDevice = mTDevice;
	// Directional light.
	
	// Directional light options for debugging Point Light
	mDirLight.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLight.Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLight.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLight.Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	
	/*mDirLight.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLight.Diffuse = XMFLOAT4(0.5f, 0.2f, 0.5f, 1.0f);
	mDirLight.Specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	mDirLight.Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);*/

	// Point light--position is changed every frame to animate in UpdateScene function.
	mPointLight.Ambient = XMFLOAT4(0.25f, 0.2f, 0.1f, 1.0f);
	mPointLight.Diffuse = XMFLOAT4(0.7f, 0.8f, 0.79f, 1.0f);
	mPointLight.Specular = XMFLOAT4(0.7f, 0.8f, 0.7f, 1.0f);
	mPointLight.Att = XMFLOAT3(1.0f, 0.0f, 0.0f);
	mPointLight.Range = 400.0f;

	// Spec light options for debugging Point Light

	mSpotLight.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mSpotLight.Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mSpotLight.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mSpotLight.Att = XMFLOAT3(1.0f, 0.0f, 0.0f);
	mSpotLight.Spot = 96.0f;
	mSpotLight.Range = 10000.0f;

	// Spot light--position and direction changed every frame to animate in UpdateScene function.
	/*mSpotLight.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mSpotLight.Diffuse = XMFLOAT4(0.0f, 0.5f, 0.8f, 1.0f);
	mSpotLight.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mSpotLight.Att = XMFLOAT3(1.0f, 0.0f, 0.0f);
	mSpotLight.Spot = 64.0f;
	mSpotLight.Range = 10000.0f;

	//Set Spotlight position - not true to original
	mSpotLight.Position = DirectX::XMFLOAT3(1.5f, 2.5f, -2.5f);
	mSpotLight.Direction = DirectX::XMFLOAT3(-1.5f, -2.5f, 2.5f);
	XMStoreFloat3(&mSpotLight.Direction, XMVector3Normalize(XMLoadFloat3(&mSpotLight.Direction)));
	//DirectX::XMVector3Normalize(XMFLOAT3(-1.5f, -2.5f, 2.5f);*/


	CubeMat.Ambient = XMFLOAT4(0.1f, 0.18f, 0.16f, 1.0f);
	CubeMat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	CubeMat.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 64.0f);
}


HRESULT ToshRenderer::CreateCBPerFrame(){
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = ((sizeof(cbPerFrame) + 15) / 16) * 16;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = mTDevice->g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pcbPerFrame);
	if (FAILED(hr))
		return hr;
}


HRESULT ToshRenderer::CreateCBPerObject(){
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = ((sizeof(cbPerObject)+15) / 16) * 16;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = mTDevice->g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pcbPerObject);
	if (FAILED(hr))
		return hr;
}

HRESULT ToshRenderer::CreateShaderRV(WCHAR *filename, ID3D11ShaderResourceView** resourceView){
	
	HRESULT hr = S_OK;
	DirectX::TexMetadata* metadata = new DirectX::TexMetadata();
	DirectX::ScratchImage* scratchImage = new DirectX::ScratchImage();
	DirectX::LoadFromDDSFile(filename, DirectX::DDS_FLAGS_NONE, metadata, *scratchImage);
	DirectX::CreateShaderResourceView(mTDevice->g_pd3dDevice, scratchImage->GetImages(), scratchImage->GetImageCount(), *metadata,
		resourceView);
	return hr;
}

HRESULT ToshRenderer::CreateCubeMapShaderRV(WCHAR *filename){

	HRESULT hr = S_OK;
	DirectX::TexMetadata* metadata = new DirectX::TexMetadata();
	DirectX::ScratchImage* scratchImage = new DirectX::ScratchImage();
	DirectX::LoadFromDDSFile(filename, DirectX::DDS_FLAGS_NONE, metadata, *scratchImage);
	DirectX::CreateShaderResourceView(mTDevice->g_pd3dDevice, scratchImage->GetImages(), scratchImage->GetImageCount(), *metadata,
		&g_pCubeMapRV);
	return hr;

}


/*ToshRenderer::ToshRenderer(D3DInitializer* Device){
	
	mTDevice = Device;
}

void ToshRenderer::Draw(){
	

}*/
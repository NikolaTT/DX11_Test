#ifndef TOSH_RENDERER_H
#define TOSH_RENDERER_H

//#include "D3DInitializer.h"
#include "LightTypes.h"
#include "ConstantBufferHelper.h"
//#include <d3d11_1.h>
#include "D3DInitializer.h"
#include "DirectXLibraries\DirectXTex\DirectXTex.h"

class ToshRenderer{

public:

	D3DInitializer* mTDevice;

	/*ToshRenderer(D3DInitializer* mTDevice);

	void Draw();
	void ClearColor(const FLOAT ColorRGBA[4]);*/

	DirectionalLight mDirLight;
	PointLight mPointLight;
	SpotLight mSpotLight;
	Material CubeMat;

	ID3D11Buffer* g_pcbPerFrame = nullptr;
	ID3D11Buffer* g_pcbPerObject = nullptr;
	ID3D11ShaderResourceView*	g_pDisplacementTextureRV = nullptr;
	ID3D11ShaderResourceView*	g_pTextureRV = nullptr;
	ID3D11ShaderResourceView*	g_pCubeMapRV = nullptr;


	ToshRenderer(D3DInitializer* mTDevice);
	~ToshRenderer();

	HRESULT CreateCBPerFrame();
	HRESULT CreateCBPerObject();
	HRESULT CreateShaderRV(WCHAR *filename, ID3D11ShaderResourceView** resourceView);
	HRESULT CreateCubeMapShaderRV(WCHAR *filename);
};

#endif
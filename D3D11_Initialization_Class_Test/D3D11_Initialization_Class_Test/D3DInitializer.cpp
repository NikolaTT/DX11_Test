#include "D3DInitializer.h"
#include <fstream>

using namespace DirectX;

HRESULT D3DInitializer::InitializeDevice(){
	HRESULT hr = S_OK;

	layout = new ToshLayout();

	RECT rc;
	GetClientRect(g_hWnd, &rc);
	width = rc.right - rc.left;
	height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(nullptr, g_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);

		if (hr == E_INVALIDARG)
		{
			// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
			hr = D3D11CreateDeviceAndSwapChain(nullptr, g_driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
				D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);
		}

		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;

	// Obtain the Direct3D 11.1 versions if available
	hr = g_pd3dDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&g_pd3dDevice1));
	if (SUCCEEDED(hr))
	{
		(void)g_pImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&g_pImmediateContext1));
	}

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr))
		return hr;

	hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pImmediateContext->RSSetViewports(1, &vp);

	return S_OK;
}


D3DInitializer::D3DInitializer(HWND mWindowHandle):g_hWnd(mWindowHandle){

	InitializeDevice();

}

D3DInitializer::~D3DInitializer(){

	if (g_pImmediateContext) g_pImmediateContext->ClearState();

	if (g_pRenderTargetView) g_pRenderTargetView->Release();
	if (g_pSwapChain) g_pSwapChain->Release();
	if (g_pImmediateContext1) g_pImmediateContext1->Release();
	if (g_pImmediateContext) g_pImmediateContext->Release();
	if (g_pd3dDevice1) g_pd3dDevice1->Release();
	if (g_pd3dDevice) g_pd3dDevice->Release();

}


//have not checked for FAIL
HRESULT D3DInitializer::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut){
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			std::ofstream fout("Shader_Debug.txt");
			fout << reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer());
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;

}

HRESULT D3DInitializer::CreateVertexShader(const void *pShaderBytecode, SIZE_T BytecodeLength,
	ID3D11ClassLinkage *pClassLinkage, ID3D11VertexShader **ppVertexShader){

	HRESULT hr = S_OK;
	
	hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &g_pVertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}
}

void D3DInitializer::AddInputLayout(LPCSTR SemanticName, UINT SemanticIndex, DXGI_FORMAT Format,
	UINT InputSlot, UINT AlignedByteOffset, D3D11_INPUT_CLASSIFICATION InputSlotClass, UINT InstanceDataStepRate){

	layout->AddInputLayout(SemanticName, SemanticIndex, Format,
		InputSlot, AlignedByteOffset, InputSlotClass, InstanceDataStepRate);

}

HRESULT D3DInitializer::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC *pInputElementDescs, UINT NumElements, const void *pShaderBytecodeWithInputSignature,
	SIZE_T BytecodeLength, ID3D11InputLayout **ppInputLayout){

	HRESULT hr;
	hr = g_pd3dDevice->CreateInputLayout(layout->mTlayout, layout->numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &g_pVertexLayout);
	pVSBlob->Release();
	if (FAILED(hr))
		return hr;

}

void D3DInitializer::setInputLayout(){
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout);
}

HRESULT D3DInitializer::CreateHullShader(const void *pShaderBytecode, SIZE_T BytecodeLength,
	ID3D11ClassLinkage *pClassLinkage, ID3D11HullShader **ppHullShader){

	HRESULT hr = S_OK;
	hr = g_pd3dDevice->CreateHullShader(pHSBlob->GetBufferPointer(), pHSBlob->GetBufferSize(), nullptr, &g_pHullShader);
	pHSBlob->Release();
	if (FAILED(hr))
		return hr;
}

HRESULT D3DInitializer::CreateDomainShader(const void *pShaderBytecode, SIZE_T BytecodeLength,
	ID3D11ClassLinkage *pClassLinkage, ID3D11DomainShader **ppHullShader){

	HRESULT hr = S_OK;
	hr = g_pd3dDevice->CreateDomainShader(pDSBlob->GetBufferPointer(), pDSBlob->GetBufferSize(), nullptr, &g_pDomainShader);
	pDSBlob->Release();
	if (FAILED(hr))
		return hr;
}



HRESULT D3DInitializer::CreatePixelShader(const void *pShaderBytecode, SIZE_T BytecodeLength,
	ID3D11ClassLinkage *pClassLinkage, ID3D11PixelShader **ppPixelShader){

	HRESULT hr = S_OK;
	hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
		return hr;

}

HRESULT D3DInitializer::CreateVertexBuffer(int numVertices){

	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VertexTypes::VertexBasic)*numVertices;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
	if (FAILED(hr))
		return hr;
}

void D3DInitializer::populateIndex(WORD indices2[], UINT numIndices){

	delete[] indices;
	indices = new WORD[numIndices];
	for (int i = 0; i < numIndices; i++){
		indices[i] = indices2[i];
	}


}


//CreateIndexBuffer number of vertices is hard coded!
HRESULT D3DInitializer::CreateIndexBuffer(){

	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD)* 36;        // 36 vertices needed for 12 triangles in a triangle list
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indices;
	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
	if (FAILED(hr))
		return hr;
}

void D3DInitializer::SetIndexBuffer(){
	g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
}

/*HRESULT D3DInitializer::CreateVertexBuffer(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2, DirectX::XMFLOAT3 p3, DirectX::XMFLOAT4 c1, DirectX::XMFLOAT4 c2, DirectX::XMFLOAT4 c3){

	HRESULT hr = S_OK;

	vertices = new VertexTypes::VertexBasic[3];

	vertices[0].Pos = p1;
	//vertices[0].Color = c1;

	vertices[1].Pos = p2;
	//vertices[1].Color = c2;

	vertices[2].Pos = p3;
	//vertices[2].Color = c3;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex)* 3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
	if (FAILED(hr))
		return hr;

}
*/

void D3DInitializer::SetVertexBuffer(){
	UINT stride = sizeof(VertexTypes::VertexBasic);
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
}

void D3DInitializer::SetPrimitiveTopology(){
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
}

void D3DInitializer::SetVertexShader(){
	g_pImmediateContext->VSSetShader(g_pVertexShader, nullptr, 0);
}

void D3DInitializer::SetPixelShader(){
	g_pImmediateContext->PSSetShader(g_pPixelShader, nullptr, 0);
}

void D3DInitializer::SetHullShader(){
	g_pImmediateContext->HSSetShader(g_pHullShader, nullptr, 0);
}

void D3DInitializer::SetDomainShader(){
	g_pImmediateContext->DSSetShader(g_pDomainShader, nullptr, 0);
}


void D3DInitializer::ClearVertices(){
	delete vertices;
	vertices = nullptr;
	numVertices = 0;
}

/*HRESULT D3DInitializer::AddVertexPosition(DirectX::XMFLOAT3 position){
	HRESULT hr = S_OK;
	if (vertices == nullptr){
		vertices = new VertexTypes::VertexBasic();
		vertices->Pos = position;
		vertices->Normal = position;
		vertices->Tex.x = position.x;
		vertices->Tex.y = position.y;
	}
	else{
		VertexTypes::VertexBasic* temp = vertices;
		vertices = new VertexTypes::VertexBasic[numVertices + 1];
		memcpy(vertices, temp, sizeof(VertexTypes::VertexBasic)*numVertices);
		delete temp;
		vertices[numVertices].Pos = position;
		vertices[numVertices].Normal = position;
		vertices[numVertices].Tex.x = position.x;
		vertices[numVertices].Tex.y = position.y;
	}
	numVertices++;
	return hr;
}*/

HRESULT D3DInitializer::AddVertexColor(DirectX::XMFLOAT4 color){
	HRESULT hr = S_OK;
	//vertices[numVertices - 1].Color = color;
	return hr;
}


HRESULT D3DInitializer::CreateConstantBuffer(){
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pConstantBuffer);
	if (FAILED(hr))
		return hr;
}

void D3DInitializer::InitializeWorldMatrix(){
	g_World1 = DirectX::XMMatrixIdentity();
	g_World2 = DirectX::XMMatrixIdentity();
}

void D3DInitializer::InitializeViewMatrix(){
	DirectX::XMVECTOR Eye = DirectX::XMVectorSet(0.0f, 20.5f, 30.5f, 0.0f);
	DirectX::XMVECTOR At = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View = DirectX::XMMatrixLookAtLH(Eye, At, Up);
}

void D3DInitializer::InitializeProjectionMatrix(){
	/*RECT rc;
	GetClientRect(g_hWnd, &rc);
	width = rc.right - rc.left;
	height = rc.bottom - rc.top;*/
	g_Projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, width / (FLOAT)height, 0.01f, 100.0f);
}

HRESULT D3DInitializer::CreateDepthStencilView(){
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = g_pd3dDevice->CreateTexture2D(&descDepth, nullptr, &g_pDepthStencil);
	if (FAILED(hr))
		return hr;

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = g_pd3dDevice->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);
	if (FAILED(hr))
		return hr;

	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
}
#include <windows.h>
#include "WindowInitialization.h"
#include "D3DInitializer.h"
#include "ToshRenderer.h"
#include <vector>
#include "VertexTypes.h"
#include "ObjModelParser.h"
#include <fstream>
#include "RenderStates.h"
#include "AlphaBlendSorter.h"

//changed VertexBasic Position to XMFLOAT4

using namespace DirectX; using std::vector; using std::ofstream;

void Render(D3DInitializer* mD3DInit, ToshRenderer* mTRenderer, RenderStates* mTRenderStates);

float rotatePointLightAngle = 0;
float lightYPositionDelta = 0;
int numVertices = 0;

float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WindowInitializer* mWinInit;
	D3DInitializer* mD3DInit;
	ToshRenderer* mTRenderer;

	ofstream fout("debug_Main.txt");
	fout << "Debug\n";

	if (FAILED(mWinInit = new WindowInitializer(hInstance, nCmdShow)))
		return 0;

	if (FAILED(mD3DInit = new D3DInitializer(mWinInit->g_hWnd)))
		return 0;

	mTRenderer = new ToshRenderer(mD3DInit);
	
	

	std::vector<VertexTypes::VertexBasic> vertices;
	std::vector<USHORT> indices;
	ObjModelParser objMP;
	objMP.LoadObj("IronMan2.obj", vertices, indices);

	

	fout << vertices.size() << '\n';

	fout << "\n\nBegin output\n";
	for (std::vector<VertexTypes::VertexBasic>::iterator iter = vertices.begin(); iter < vertices.end(); iter++){
		fout << "Pos: " << iter->Pos.x << " " << iter->Pos.y << " " << iter->Pos.z << '\n';
		//fout << "Tex: " << iter->Tex.x << " " << iter->Tex.y << '\n';
		//fout << "Norm: " << iter->Normal.x << " " << iter->Normal.y << " " << iter->Normal.z << '\n';
	}

	fout.flush();

	mD3DInit->CreateDepthStencilView();

	mD3DInit->CompileShaderFromFile(L"MyFirstVS.hlsl", "VS", "vs_4_0", &(mD3DInit->pVSBlob));

	mD3DInit->CreateVertexShader(mD3DInit->pVSBlob->GetBufferPointer(), mD3DInit->pVSBlob->GetBufferSize(), nullptr, &(mD3DInit->g_pVertexShader));

	mD3DInit->AddInputLayout("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);

	mD3DInit->AddInputLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);

	mD3DInit->AddInputLayout("TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0);

	mD3DInit->AddInputLayout("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0);

	mD3DInit->AddInputLayout("DIS_TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0);

	mD3DInit->CreateInputLayout(mD3DInit->layout->mTlayout, mD3DInit->layout->numElements, mD3DInit->pVSBlob->GetBufferPointer(),
		mD3DInit->pVSBlob->GetBufferSize(), &(mD3DInit->g_pVertexLayout));

	mD3DInit->setInputLayout();

	mD3DInit->CompileShaderFromFile(L"MyFirstPS.hlsl", "PS", "ps_4_0", &(mD3DInit->pPSBlob));

	mD3DInit->CreatePixelShader(mD3DInit->pPSBlob->GetBufferPointer(), mD3DInit->pPSBlob->GetBufferSize(), nullptr,
		&mD3DInit->g_pPixelShader);

	mD3DInit->CompileShaderFromFile(L"MyFirstHS.hlsl", "HS", "hs_5_0", &(mD3DInit->pHSBlob));

	mD3DInit->CreateHullShader(mD3DInit->pHSBlob->GetBufferPointer(), mD3DInit->pHSBlob->GetBufferSize(), nullptr,
		&mD3DInit->g_pHullShader);

	mD3DInit->CompileShaderFromFile(L"MyFirstDS.hlsl", "DS", "ds_5_0", &(mD3DInit->pDSBlob));

	mD3DInit->CreateDomainShader(mD3DInit->pDSBlob->GetBufferPointer(), mD3DInit->pDSBlob->GetBufferSize(), nullptr,
		&mD3DInit->g_pDomainShader);

	mD3DInit->InitializeWorldMatrix();

	mD3DInit->InitializeViewMatrix();

	mD3DInit->InitializeProjectionMatrix();


	fout << "\n\nBegin output\n";
	for (std::vector<VertexTypes::VertexBasic>::iterator iter = vertices.begin(); iter < vertices.end(); iter++){
		fout << "Pos: " << iter->Pos.x << " " << iter->Pos.y << " " << iter->Pos.z << " " << iter->Pos.w << '\n';
		//fout << "Tex: " << iter->Tex.x << " " << iter->Tex.y << '\n';
		//fout << "Norm: " << iter->Normal.x << " " << iter->Normal.y << " " << iter->Normal.z << '\n';
	}

	fout.flush();

	AlphaBlendSorter alphaBlendSorter;
	alphaBlendSorter.sortVertices(vertices, mD3DInit->g_World1, mD3DInit->g_View, mD3DInit->g_Projection);

	fout << "\n\nBegin output\n";
	for (std::vector<VertexTypes::VertexBasic>::iterator iter = vertices.begin(); iter < vertices.end(); iter++){
		
		float Dist = 0.0f;
		Dist += iter->Pos.z;
		iter++;
		Dist += iter->Pos.z;
		iter++;
		Dist += iter->Pos.z;

		fout << "Distance: " << Dist << '\n';
		//fout << "Pos: " << iter->Pos.x << " " << iter->Pos.y << " " << iter->Pos.z << " " << iter->Pos.w << '\n';
		//fout << "Tex: " << iter->Tex.x << " " << iter->Tex.y << '\n';
		//fout << "Norm: " << iter->Normal.x << " " << iter->Normal.y << " " << iter->Normal.z << '\n';
	}

	fout.flush();

	numVertices = vertices.size();

	mD3DInit->vertices = new VertexTypes::VertexBasic[vertices.size()];
	for (int i = 0; i < vertices.size(); i++){
		
		mD3DInit->vertices[i] = vertices[i];

		/*mD3DInit->vertices[i].Pos.x = rand() / (float)RAND_MAX;
		mD3DInit->vertices[i].Pos.y = rand() / (float)RAND_MAX;
		mD3DInit->vertices[i].Pos.z = rand() / (float)RAND_MAX;*/
	}

	mD3DInit->CreateVertexBuffer(numVertices);

	mD3DInit->SetVertexBuffer();

	mD3DInit->SetPrimitiveTopology();

	mD3DInit->CreateConstantBuffer();

	

	

	RenderStates* mTRenderStates = new RenderStates();
	mTRenderStates->InitAll(mD3DInit->g_pd3dDevice);

	mD3DInit->SetVertexShader();

	mD3DInit->SetPixelShader();

	mD3DInit->SetHullShader();

	mD3DInit->SetDomainShader();

	mTRenderer->CreateCBPerFrame();
	mTRenderer->CreateCBPerObject();
	mTRenderer->CreateShaderRV(L"DisplacementMap3.DDS", &mTRenderer->g_pDisplacementTextureRV);
	mTRenderer->CreateShaderRV(L"WireFence.dds", &mTRenderer->g_pTextureRV);
	mTRenderer->CreateCubeMapShaderRV(L"desertcube1024.dds");


	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Render(mD3DInit, mTRenderer, mTRenderStates);
		}
	}


	return (int)msg.wParam;
	
	
}


void Render(D3DInitializer* mD3DInitializer, ToshRenderer* mTRenderer, RenderStates* mTRenderStates)
{
	// Now set the rasterizer state and blend state.
	// Blend factor global constant hardcoded

	mD3DInitializer->g_pImmediateContext->RSSetState(mTRenderStates->NoCullRS);
	mD3DInitializer->g_pImmediateContext->OMSetBlendState(mTRenderStates->TransparentBS, blendFactor, 0xffffffff);
	mD3DInitializer->g_pImmediateContext->OMSetDepthStencilState(mTRenderStates->TutorialDDS, 1.0f);
	



	//
	// Clear the back buffer
	//
	mD3DInitializer->g_pImmediateContext->ClearRenderTargetView(mD3DInitializer->g_pRenderTargetView, DirectX::Colors::White);

	//
	// Clear the depth buffer to 1.0 (max depth)
	//
	mD3DInitializer->g_pImmediateContext->ClearDepthStencilView(mD3DInitializer->g_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0);

	ConstantBuffer cb1;
	cb1.mWorld = XMMatrixTranspose(mD3DInitializer->g_World1);
	cb1.mView = XMMatrixTranspose(mD3DInitializer->g_View);
	cb1.mProjection = XMMatrixTranspose(mD3DInitializer->g_Projection);
	mD3DInitializer->g_pImmediateContext->UpdateSubresource(mD3DInitializer->g_pConstantBuffer, 0, nullptr, &cb1, 0, 0);

	cbPerFrame v_cbPerFrame;
	cbPerObject v_cbPerObject;
	
	
	//Set Per Frame Constant Buffer
	
	v_cbPerFrame.gDirLight = mTRenderer->mDirLight;
	v_cbPerFrame.gEyePosW = DirectX::XMFLOAT3(1.5f, 2.5f, -2.5f);
	v_cbPerFrame.gPointLight = mTRenderer->mPointLight;
	v_cbPerFrame.gSpotLight = mTRenderer->mSpotLight;
	v_cbPerFrame.gPointLight.Position.x = 0.0f;//70.0f*cosf(0.02f*rotatePointLightAngle);
	v_cbPerFrame.gPointLight.Position.z = 20.5f;//70.0f*sinf(0.02f*rotatePointLightAngle);
	v_cbPerFrame.gPointLight.Position.y = 30.5f;
	
	/*
	//Comment out point light rotation
	XMVECTOR gPointLightPos = XMLoadFloat3(&v_cbPerFrame.gPointLight.Position);
	gPointLightPos = XMVector3Transform(gPointLightPos, XMMatrixRotationY(-rotatePointLightAngle));
	XMStoreFloat3(&v_cbPerFrame.gPointLight.Position, gPointLightPos);
	//v_cbPerFrame.gPointLight.Position.y += 10;*/

	rotatePointLightAngle += 0.000030;

	
	//Set Per Object Constant Buffer
	v_cbPerObject.gMaterial = mTRenderer->CubeMat;

	mD3DInitializer->g_World1 = XMMatrixRotationY(rotatePointLightAngle);

	v_cbPerObject.gWorld = XMMatrixTranspose(mD3DInitializer->g_World1);

	//Not true to original
	DirectX::XMVECTOR det = XMMatrixDeterminant(v_cbPerObject.gWorld);
	v_cbPerObject.gWorldInvTranspose = XMMatrixInverse(&det, v_cbPerObject.gWorld);
	v_cbPerObject.gWorldInvTranspose = XMMatrixTranspose(v_cbPerObject.gWorldInvTranspose);

	v_cbPerObject.gWorldViewProj = XMMatrixTranspose(mD3DInitializer->g_World1 * mD3DInitializer->g_View * mD3DInitializer->g_Projection);
	v_cbPerObject.gTexTransform = DirectX::XMMatrixIdentity();
	v_cbPerObject.gViewProj = XMMatrixTranspose(mD3DInitializer->g_View * mD3DInitializer->g_Projection);

	//
	//End Constant Buffer Initialization
	//
	
	
	//Set VS Constant Buffer
	mD3DInitializer->g_pImmediateContext->UpdateSubresource(mTRenderer->g_pcbPerFrame, 0, nullptr, &v_cbPerFrame, 0, 0);
	mD3DInitializer->g_pImmediateContext->VSSetConstantBuffers(1, 1, &mTRenderer->g_pcbPerFrame);

	mD3DInitializer->g_pImmediateContext->UpdateSubresource(mTRenderer->g_pcbPerObject, 0, nullptr, &v_cbPerObject, 0, 0);
	mD3DInitializer->g_pImmediateContext->VSSetConstantBuffers(2, 1, &mTRenderer->g_pcbPerObject);

	
	
	//Set PS Constant Buffer
	mD3DInitializer->g_pImmediateContext->UpdateSubresource(mTRenderer->g_pcbPerFrame, 0, nullptr, &v_cbPerFrame, 0, 0);
	mD3DInitializer->g_pImmediateContext->PSSetConstantBuffers(1, 1, &mTRenderer->g_pcbPerFrame);

	mD3DInitializer->g_pImmediateContext->UpdateSubresource(mTRenderer->g_pcbPerObject, 0, nullptr, &v_cbPerObject, 0, 0);
	mD3DInitializer->g_pImmediateContext->PSSetConstantBuffers(2, 1, &mTRenderer->g_pcbPerObject);

	//Set PS Texture

	mD3DInitializer->g_pImmediateContext->PSSetShaderResources(0, 1, &mTRenderer->g_pCubeMapRV);
	mD3DInitializer->g_pImmediateContext->PSSetShaderResources(1, 1, &mTRenderer->g_pTextureRV);

	

	//mD3DInitializer->g_pImmediateContext->VSSetConstantBuffers(0, 1, &mD3DInitializer->g_pConstantBuffer);
	//mD3DInitializer->g_pImmediateContext->HSSetConstantBuffers(0, 1, &mD3DInitializer->g_pConstantBuffer);
	//mD3DInitializer->g_pImmediateContext->DSSetConstantBuffers(0, 1, &mD3DInitializer->g_pConstantBuffer);
	
	
	
	
	//Set DS Constant Buffer
	mD3DInitializer->g_pImmediateContext->UpdateSubresource(mTRenderer->g_pcbPerFrame, 0, nullptr, &v_cbPerFrame, 0, 0);
	mD3DInitializer->g_pImmediateContext->DSSetConstantBuffers(1, 1, &mTRenderer->g_pcbPerFrame);
	
	mD3DInitializer->g_pImmediateContext->UpdateSubresource(mTRenderer->g_pcbPerObject, 0, nullptr, &v_cbPerObject, 0, 0);
	mD3DInitializer->g_pImmediateContext->DSSetConstantBuffers(2, 1, &mTRenderer->g_pcbPerObject);

	//Set DS Texture

	mD3DInitializer->g_pImmediateContext->DSSetShaderResources(0, 1, &mTRenderer->g_pDisplacementTextureRV);



	mD3DInitializer->g_pImmediateContext->Draw(numVertices, 0);

	mD3DInitializer->g_pSwapChain->Present(0, 0);
}



/*// Update our time
static float t = 0.0f;
if (mD3DInitializer->g_driverType == D3D_DRIVER_TYPE_REFERENCE)
{
t += (float)XM_PI * 0.0125f;
}
else
{
static ULONGLONG timeStart = 0;
ULONGLONG timeCur = GetTickCount64();
if (timeStart == 0)
timeStart = timeCur;
t = (timeCur - timeStart) / 1000.0f;
}

// 1st Cube: Rotate around the origin
mD3DInitializer->g_World1 = XMMatrixRotationY(t);

// 2nd Cube:  Rotate around origin
XMMATRIX mSpin = XMMatrixRotationZ(-t);
XMMATRIX mOrbit = XMMatrixRotationY(-t * 2.0f);
XMMATRIX mTranslate = XMMatrixTranslation(-4.0f, 0.0f, 0.0f);
XMMATRIX mScale = XMMatrixScaling(0.3f, 0.3f, 0.3f);

mD3DInitializer->g_World2 = mScale * mSpin * mTranslate * mOrbit;

//
// Clear the back buffer
//
mD3DInitializer->g_pImmediateContext->ClearRenderTargetView(mD3DInitializer->g_pRenderTargetView, Colors::MidnightBlue);

//
// Clear the depth buffer to 1.0 (max depth)
//
mD3DInitializer->g_pImmediateContext->ClearDepthStencilView(mD3DInitializer->g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

//
// Update variables for the first cube
//
ConstantBuffer cb1;
cb1.mWorld = XMMatrixTranspose(mD3DInitializer->g_World1);
cb1.mView = XMMatrixTranspose(mD3DInitializer->g_View);
cb1.mProjection = XMMatrixTranspose(mD3DInitializer->g_Projection);
mD3DInitializer->g_pImmediateContext->UpdateSubresource(mD3DInitializer->g_pConstantBuffer, 0, nullptr, &cb1, 0, 0);

//
// Render the first cube
//
mD3DInitializer->g_pImmediateContext->VSSetShader(mD3DInitializer->g_pVertexShader, nullptr, 0);
mD3DInitializer->g_pImmediateContext->VSSetConstantBuffers(0, 1, &mD3DInitializer->g_pConstantBuffer);
mD3DInitializer->g_pImmediateContext->PSSetShader(mD3DInitializer->g_pPixelShader, nullptr, 0);
mD3DInitializer->g_pImmediateContext->DrawIndexed(36, 0, 0);

//
// Update variables for the second cube
//
ConstantBuffer cb2;
cb2.mWorld = XMMatrixTranspose(mD3DInitializer->g_World2);
cb2.mView = XMMatrixTranspose(mD3DInitializer->g_View);
cb2.mProjection = XMMatrixTranspose(mD3DInitializer->g_Projection);
mD3DInitializer->g_pImmediateContext->UpdateSubresource(mD3DInitializer->g_pConstantBuffer, 0, nullptr, &cb2, 0, 0);

//
// Render the second cube
//
mD3DInitializer->g_pImmediateContext->DrawIndexed(36, 0, 0);

//
// Present our back buffer to our front buffer
//
mD3DInitializer->g_pSwapChain->Present(0, 0);*/

//
//
//
//
//

/*mD3DInit->CreateDepthStencilView();

mD3DInit->CompileShaderFromFile(L"Tutorial05_VS.hlsl", "VS", "vs_4_0", &(mD3DInit->pVSBlob));

mD3DInit->CreateVertexShader(mD3DInit->pVSBlob->GetBufferPointer(), mD3DInit->pVSBlob->GetBufferSize(), nullptr, &(mD3DInit->g_pVertexShader));

mD3DInit->AddInputLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);

mD3DInit->AddInputLayout("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);

mD3DInit->CreateInputLayout(mD3DInit->layout->mTlayout, mD3DInit->layout->numElements, mD3DInit->pVSBlob->GetBufferPointer(),
mD3DInit->pVSBlob->GetBufferSize(), &(mD3DInit->g_pVertexLayout));

mD3DInit->setInputLayout();

mD3DInit->CompileShaderFromFile(L"Tutorial05_PS.hlsl", "PS", "ps_4_0", &(mD3DInit->pPSBlob));

mD3DInit->CreatePixelShader(mD3DInit->pPSBlob->GetBufferPointer(), mD3DInit->pPSBlob->GetBufferSize(), nullptr,
&mD3DInit->g_pPixelShader);

mD3DInit->AddVertexPosition(XMFLOAT3(-1.0f, 1.0f, -1.0f)); mD3DInit->AddVertexColor(XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
mD3DInit->AddVertexPosition(XMFLOAT3(1.0f, 1.0f, -1.0f)); mD3DInit->AddVertexColor(XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
mD3DInit->AddVertexPosition(XMFLOAT3(1.0f, 1.0f, 1.0f)); mD3DInit->AddVertexColor(XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f));
mD3DInit->AddVertexPosition(XMFLOAT3(-1.0f, 1.0f, 1.0f)); mD3DInit->AddVertexColor(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
mD3DInit->AddVertexPosition(XMFLOAT3(-1.0f, -1.0f, -1.0f)); mD3DInit->AddVertexColor(XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f));
mD3DInit->AddVertexPosition(XMFLOAT3(1.0f, -1.0f, -1.0f)); mD3DInit->AddVertexColor(XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
mD3DInit->AddVertexPosition(XMFLOAT3(1.0f, -1.0f, 1.0f)); mD3DInit->AddVertexColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
mD3DInit->AddVertexPosition(XMFLOAT3(-1.0f, -1.0f, 1.0f)); mD3DInit->AddVertexColor(XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));





mD3DInit->CreateVertexBuffer();

mD3DInit->SetVertexBuffer();

mD3DInit->indices = new WORD[36];

WORD indices[] = {
3, 1, 0,
2, 1, 3,

0, 5, 4,
1, 5, 0,

3, 4, 7,
0, 4, 3,

1, 6, 5,
2, 6, 1,

2, 7, 6,
3, 7, 2,

6, 4, 5,
7, 4, 6,
};

mD3DInit->populateIndex(indices, 36);

mD3DInit->CreateIndexBuffer();

mD3DInit->SetIndexBuffer();

mD3DInit->SetPrimitiveTopology();

mD3DInit->CreateConstantBuffer();

mD3DInit->InitializeWorldMatrix();

mD3DInit->InitializeViewMatrix();

mD3DInit->InitializeProjectionMatrix();

//mD3DInit->SetVertexShader();

//mD3DInit->SetPixelShader();


MSG msg = { 0 };
while (WM_QUIT != msg.message)
{
if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
{
TranslateMessage(&msg);
DispatchMessage(&msg);
}
else
{
Render(mD3DInit);
}
}


return (int)msg.wParam;*/









/*std::ofstream fout("name.txt");
fout << "Begin output\n";
for (std::vector<VertexTypes::VertexBasic>::iterator iter = vertices.begin(); iter < vertices.end(); iter++){
fout << "Pos: " << iter->Pos.x << " " << iter->Pos.y << " " << iter->Pos.z << '\n';
fout << "Tex: " << iter->Tex.x << " " << iter->Tex.y << '\n';
fout << "Norm: " << iter->Normal.x << " " << iter->Normal.y << " " << iter->Normal.z << '\n';
}*/

/*if (FAILED(InitWindow(hInstance, nCmdShow)))
return 0;

if (FAILED(InitDevice()))
{
CleanupDevice();
return 0;
}

// Main message loop
MSG msg = { 0 };
while (WM_QUIT != msg.message)
{
if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
{
TranslateMessage(&msg);
DispatchMessage(&msg);
}
else
{
Render();
}
}

CleanupDevice();*/


/*//HARDCODED
	D3D11_RASTERIZER_DESC rasterDesc;
	ID3D11RasterizerState* m_rasterState = 0;

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	HRESULT result = mD3DInitializer->g_pd3dDevice->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result))
	{
		return;
		}*/
#ifndef D3D_INITIALIZER_H
#define D3D_INITIALIZER_H

#include <d3d11_1.h>
#include <directxcolors.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <winnt.h>
#include "VertexTypes.h"
#include "LightTypes.h"

struct ToshLayout{

	D3D11_INPUT_ELEMENT_DESC* mTlayout;
	UINT numElements = 0;

	void eraseInputLayout(){
		delete mTlayout;
		mTlayout = nullptr;
		numElements = 0;
	}

	void AddInputLayout(LPCSTR SemanticName, UINT SemanticIndex, DXGI_FORMAT Format,
		UINT InputSlot, UINT AlignedByteOffset, D3D11_INPUT_CLASSIFICATION InputSlotClass, UINT InstanceDataStepRate){

		if (numElements==0){
			mTlayout = new D3D11_INPUT_ELEMENT_DESC();
			mTlayout->SemanticName = SemanticName;
			mTlayout->SemanticIndex = SemanticIndex;
			mTlayout->Format = Format;
			mTlayout->InputSlot = InputSlot;
			mTlayout->AlignedByteOffset = AlignedByteOffset;
			mTlayout->InputSlotClass = InputSlotClass;
			mTlayout->InstanceDataStepRate = InstanceDataStepRate;
		}
		else{
			D3D11_INPUT_ELEMENT_DESC* layoutTemp = mTlayout;
			mTlayout = new D3D11_INPUT_ELEMENT_DESC[numElements + 1];
			memcpy(mTlayout, layoutTemp, numElements*sizeof(D3D11_INPUT_ELEMENT_DESC));
			delete layoutTemp;
			mTlayout[numElements].SemanticName = SemanticName;
			mTlayout[numElements].SemanticIndex = SemanticIndex;
			mTlayout[numElements].Format = Format;
			mTlayout[numElements].InputSlot = InputSlot;
			mTlayout[numElements].AlignedByteOffset = AlignedByteOffset;
			mTlayout[numElements].InputSlotClass = InputSlotClass;
			mTlayout[numElements].InstanceDataStepRate = InstanceDataStepRate;
		}

		numElements++;
		/*layout = new D3D11_INPUT_ELEMENT_DESC();
		layout->SemanticName = SemanticName;
		layout->SemanticIndex = SemanticIndex;
		layout->Format = Format;
		layout->InputSlot = InputSlot;
		layout->AlignedByteOffset = AlignedByteOffset;
		layout->InputSlotClass = InputSlotClass;
		layout->InstanceDataStepRate = InstanceDataStepRate;*/
	}


};

struct SimpleVertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT4 Color;
};


struct ConstantBuffer
{
	DirectX::XMMATRIX mWorld;
	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProjection;
};




class D3DInitializer{

public:
	HWND                    g_hWnd = nullptr;
	D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
	ID3D11Device*           g_pd3dDevice = nullptr;
	IDXGISwapChain*         g_pSwapChain = nullptr;
	D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11DeviceContext*    g_pImmediateContext = nullptr;
	ID3D11Device1*          g_pd3dDevice1 = nullptr;
	ID3D11DeviceContext1*   g_pImmediateContext1 = nullptr;
	ID3D11RenderTargetView* g_pRenderTargetView = nullptr;
	ID3D11Texture2D*        g_pDepthStencil = nullptr;

	UINT width = 0;
	UINT height = 0;

	ID3D11VertexShader* g_pVertexShader;
	ID3D11InputLayout* g_pVertexLayout = nullptr;
	ID3D11Buffer* g_pVertexBuffer = nullptr;
	ID3D11Buffer* g_pIndexBuffer = nullptr;
	ID3D11PixelShader* g_pPixelShader;
	ID3D11HullShader* g_pHullShader;
	ID3D11DomainShader* g_pDomainShader;
	ID3DBlob* pVSBlob = nullptr;
	ID3DBlob* pPSBlob = nullptr;
	ID3DBlob* pHSBlob = nullptr;
	ID3DBlob* pDSBlob = nullptr;
	

	//ToshRenderer* mToshRenderer = nullptr;


	ID3D11Buffer* g_pConstantBuffer = nullptr;
	

	DirectX::XMMATRIX g_World1;
	DirectX::XMMATRIX g_World2;
	DirectX::XMMATRIX g_View;
	DirectX::XMMATRIX g_Projection;

	ID3D11DepthStencilView* g_pDepthStencilView = nullptr;

	ToshLayout* layout;
	
	VertexTypes::VertexBasic* vertices = nullptr;
	UINT numVertices = 0;
	
	WORD* indices = nullptr;



	D3DInitializer(HWND mWindowHandle);
	~D3DInitializer();

	HRESULT InitializeDevice();

	

	void AddInputLayout(LPCSTR SemanticName, UINT SemanticIndex, DXGI_FORMAT Format,
		UINT InputSlot, UINT AlignedByteOffset, D3D11_INPUT_CLASSIFICATION InputSlotClass, UINT InstanceDataStepRate);
	HRESULT CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC *pInputElementDescs, UINT NumElements, const void *pShaderBytecodeWithInputSignature,
		SIZE_T BytecodeLength, ID3D11InputLayout **ppInputLayout);
	void setInputLayout();

	HRESULT CreateVertexBuffer(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2, DirectX::XMFLOAT3 p3, DirectX::XMFLOAT4 c1, DirectX::XMFLOAT4 c2, DirectX::XMFLOAT4 c3);
	HRESULT CreateVertexBuffer(int numVertices);
	HRESULT AddVertexPosition(DirectX::XMFLOAT3 position);
	HRESULT AddVertexColor(DirectX::XMFLOAT4 color);

	void populateIndex(WORD indices[], UINT numIndices);
	HRESULT CreateIndexBuffer();
	void SetIndexBuffer();

	void SetVertexBuffer();
	void SetPrimitiveTopology();

	void ClearVertices();

	HRESULT CompileShaderFromFile(WCHAR* name, LPCSTR EntryPoint, LPCSTR ShaderModel, ID3DBlob** Blob);
	HRESULT CreateVertexShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11VertexShader **ppVertexShader);
	HRESULT CreatePixelShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11PixelShader **ppPixelShader);
	HRESULT CreateHullShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11HullShader **ppHullShader);
	HRESULT CreateDomainShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11DomainShader **ppDomainShader);

	void SetVertexShader();
	void SetPixelShader();
	void SetHullShader();
	void SetDomainShader();
	

	void releaseBlob(ID3DBlob* blob);

	HRESULT CreateConstantBuffer();

	void InitializeWorldMatrix();
	void InitializeViewMatrix();
	void InitializeProjectionMatrix();

	HRESULT CreateDepthStencilView();


	/*void setRenderer(ToshRenderer* tRend){
		mToshRenderer = tRend;
	}*/
};

#endif
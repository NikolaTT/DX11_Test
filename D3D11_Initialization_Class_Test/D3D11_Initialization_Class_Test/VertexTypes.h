#ifndef VERTEXTYPES_H

#define VERTEXTYPES_H

#include <DirectXMath.h>
#include <vector>


namespace VertexTypes {

	struct VertexBasic{

		DirectX::XMFLOAT4 Pos;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT2 Tex;
		DirectX::XMFLOAT3 Tangent;
		DirectX::XMFLOAT2 DisplacementTex;

	};

	struct VertexBasicFace
	{
		VertexBasic faceVertices[3];
	};

	struct PosNormalTexTan
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT2 Tex;
		DirectX::XMFLOAT4 TangentU;
	};
}

#endif
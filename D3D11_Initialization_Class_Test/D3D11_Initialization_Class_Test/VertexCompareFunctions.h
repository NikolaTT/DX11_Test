#ifndef H_VERTEXCOMPAREFUNCTIONS
#define H_VERTEXCOMPAREFUNCTIONS

#include "VertexTypes.h"
#include <DirectXMath.h>

namespace VectorCompareFunctions{

	bool VertexBasicDistance(VertexTypes::VertexBasicFace v1, VertexTypes::VertexBasicFace v2){

		float maxDistV1 = 0;
		float maxDistV2 = 0;

		for (int i = 0; i < 3; i++){

			//if (v1.faceVertices[i].Pos.z > maxDistV1)
				maxDistV1 += v1.faceVertices[i].Pos.z;

			//if (v2.faceVertices[i].Pos.z > maxDistV2)
				maxDistV2 += v2.faceVertices[i].Pos.z;


			//DirectX::XMFLOAT4 distV1;
			//DirectX::XMStoreFloat4(&distV1, DirectX::XMVector4Length(DirectX::XMLoadFloat4(&v1.faceVertices[i].Pos)));
			//if (distV1.x > maxDistV1)
				//maxDistV1 = distV1.x;

			//DirectX::XMFLOAT4 distV2;
			//DirectX::XMStoreFloat4(&distV2, DirectX::XMVector4Length(DirectX::XMLoadFloat4(&v2.faceVertices[i].Pos)));
			//if (distV2.x > maxDistV1)
				//maxDistV2 = distV2.x;			
		}

		if (maxDistV1 < maxDistV2)
			return false;
		return true;

	}

}


/*for (int i = 0; i < vertices.size(); i++){
		XMVECTOR verticeVector;
		verticeVector = XMLoadFloat3(&vertices[i].Pos);
		XMMATRIX WorldViewProj = mD3DInit->g_World1 * mD3DInit->g_View * mD3DInit->g_Projection;
		verticeVector = XMVector4Transform(verticeVector, WorldViewProj);
		XMStoreFloat3(&vertices[i].Pos, verticeVector);
	}

	fout << vertices.size() << '\n';

	fout << "Begin output\n";
	for (std::vector<VertexTypes::VertexBasic>::iterator iter = vertices.begin(); iter < vertices.end(); iter++){
		fout << "Pos: " << iter->Pos.x << " " << iter->Pos.y << " " << iter->Pos.z << '\n';
		//fout << "Tex: " << iter->Tex.x << " " << iter->Tex.y << '\n';
		//fout << "Norm: " << iter->Normal.x << " " << iter->Normal.y << " " << iter->Normal.z << '\n';
	}

	fout.flush();

	for (int i = 0; i < vertices.size(); i++){
		VertexTypes::VertexBasicFace vbf;
		vbf.faceVertices[0] = vertices[i++];
		vbf.faceVertices[1] = vertices[i++];
		vbf.faceVertices[2] = vertices[i];
		faceVertices.push_back(vbf);
	}

	fout << "\n Face Verticews \n";

	for (int i = 0; i < faceVertices.size(); i++){
		fout << faceVertices[i].faceVertices[0].Pos.x << " " << faceVertices[i].faceVertices[0].Pos.y << " " << faceVertices[i].faceVertices[0].Pos.z;
		fout << '\n';
		fout << faceVertices[i].faceVertices[1].Pos.x << " " << faceVertices[i].faceVertices[1].Pos.y << " " << faceVertices[i].faceVertices[1].Pos.z;
		fout << '\n';
		fout << faceVertices[i].faceVertices[2].Pos.x << " " << faceVertices[i].faceVertices[2].Pos.y << " " << faceVertices[i].faceVertices[2].Pos.z;
		fout << '\n';
	}
	fout.flush();

	//sort(faceVertices.begin(), faceVertices.end(), VectorCompareFunctions::VertexBasicDistance);
	vertices.resize(0);

	for (int i = 0; i < faceVertices.size(); i++){
		VertexTypes::VertexBasicFace vbf;
		vbf = faceVertices[i];
		vertices.push_back(vbf.faceVertices[0]);
		vertices.push_back(vbf.faceVertices[1]);
		vertices.push_back(vbf.faceVertices[2]);
	}
	
	fout << vertices.size() << '\n';

	fout << "Begin output\n";
	for (std::vector<VertexTypes::VertexBasic>::iterator iter = vertices.begin(); iter < vertices.end(); iter++){
		fout << "Pos: " << iter->Pos.x << " " << iter->Pos.y << " " << iter->Pos.z << '\n';
		//fout << "Tex: " << iter->Tex.x << " " << iter->Tex.y << '\n';
		//fout << "Norm: " << iter->Normal.x << " " << iter->Normal.y << " " << iter->Normal.z << '\n';
	}

	fout.flush();*/





#endif
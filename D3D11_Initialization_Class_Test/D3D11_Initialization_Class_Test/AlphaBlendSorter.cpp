#include "AlphaBlendSorter.h"
#include <algorithm>
#include "VertexCompareFunctions.h"
#include <fstream>

using std::sort;

void AlphaBlendSorter::sortVertices(std::vector<VertexTypes::VertexBasic>& vertices, const DirectX::XMMATRIX &World,
	const DirectX::XMMATRIX &View, const DirectX::XMMATRIX &Projection){

	DirectX::XMFLOAT4X4 floatWorld;
	DirectX::XMStoreFloat4x4(&floatWorld, World);
	DirectX::XMFLOAT4X4 floatView;
	DirectX::XMStoreFloat4x4(&floatView, View);
	DirectX::XMFLOAT4X4 floatProjection;
	DirectX::XMStoreFloat4x4(&floatProjection, Projection);

	std::ofstream fout("Matrix.txt");
	fout << "World:\n\n";
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			fout << floatWorld(i, j) << " ";
		}
		fout << '\n';
	}

	fout << "\n\nView:\n\n";
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			fout << floatView(i, j) << " ";
		}
		fout << '\n';
	}

	fout << "\n\nProjection:\n\n";
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			fout << floatProjection(i, j) << " ";
		}
		fout << '\n';
	}

	DirectX::XMMATRIX worldViewProjection = World * View * Projection;
	//DirectX::XMMATRIX worldViewProjection = DirectX::XMMatrixMultiply(World, View);
	//worldViewProjection = DirectX::XMMatrixMultiply(worldViewProjection, Projection);
	//worldViewProjection = DirectX::XMMatrixTranspose(worldViewProjection);

	DirectX::XMFLOAT4X4 worldViewProjectionFloat;
	DirectX::XMStoreFloat4x4(&worldViewProjectionFloat, worldViewProjection);


	fout << "\n\nWorldViewProjection:\n\n";
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			fout << worldViewProjectionFloat(i, j) << " ";
		}
		fout << '\n';
	}



	for (int i = 0; i < vertices.size(); i++){
		DirectX::XMVECTOR verticeVector = DirectX::XMLoadFloat4(&vertices[i].Pos);
		verticeVector = DirectX::XMVector4Transform(verticeVector, worldViewProjection);
		DirectX::XMStoreFloat4(&vertices[i].Pos, verticeVector);
	}

	std::vector<VertexTypes::VertexBasicFace> vertexFaces;
	for (int i = 0; i < vertices.size(); i++){
		VertexTypes::VertexBasicFace vbf;
		vbf.faceVertices[0] = vertices[i++];
		vbf.faceVertices[1] = vertices[i++];
		vbf.faceVertices[2] = vertices[i];
		vertexFaces.push_back(vbf);
	}

	sort(vertexFaces.begin(), vertexFaces.end(), VectorCompareFunctions::VertexBasicDistance);
	vertices.resize(0);

	for (int i = 0; i < vertexFaces.size(); i++){
		VertexTypes::VertexBasicFace vbf;
		vbf = vertexFaces[i];
		vertices.push_back(vbf.faceVertices[0]);
		vertices.push_back(vbf.faceVertices[1]);
		vertices.push_back(vbf.faceVertices[2]);
	}

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




}
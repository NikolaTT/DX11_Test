#ifndef H_OBJMODELPARSER
#define H_OBJMODELPARSER

#include <vector>
#include <string>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include "VertexTypes.h"

class ObjModelParser
{
public:
	/*bool LoadM3d(const std::string& filename,
		std::vector<VertexTypes::VertexBasic>& vertices,
		std::vector<USHORT>& indices,
		std::vector<MeshGeometry::Subset>& subsets,
		std::vector<M3dMaterial>& mats);*/

	bool LoadObj(const std::string& filename,
		std::vector<VertexTypes::VertexBasic>& vertices,
		std::vector<USHORT>& indices);

	void parseFaces(std::string input, std::vector<UINT>& faceVertices, std::vector<UINT>& faceUV, std::vector<UINT>& faceNormals);
	void parseFaceVertex(std::string input, std::vector<UINT>& faceVertices, std::vector<UINT>& faceUV, std::vector<UINT>& faceNormals);


private:
	/*void ReadMaterials(std::ifstream& fin, UINT numMaterials, std::vector<M3dMaterial>& mats);
	void ReadSubsetTable(std::ifstream& fin, UINT numSubsets, std::vector<MeshGeometry::Subset>& subsets);
	void ReadVertices(std::ifstream& fin, UINT numVertices, std::vector<Vertex::PosNormalTexTan>& vertices);
	void ReadTriangles(std::ifstream& fin, UINT numTriangles, std::vector<USHORT>& indices);*/
};

#endif
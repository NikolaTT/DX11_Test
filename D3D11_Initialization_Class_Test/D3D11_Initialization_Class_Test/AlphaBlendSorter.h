#include <vector>
#include "VertexTypes.h"
#include <DirectXMath.h>

class AlphaBlendSorter{

public:

	std::vector<VertexTypes::VertexBasic> vertices;
	AlphaBlendSorter(std::vector<VertexTypes::VertexBasic> vertices);
	AlphaBlendSorter(){};
	void sortVertices(std::vector<VertexTypes::VertexBasic>& vertices, const DirectX::XMMATRIX &World,
		const DirectX::XMMATRIX &View, const DirectX::XMMATRIX &Projection);
};
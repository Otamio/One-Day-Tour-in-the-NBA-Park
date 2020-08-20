#ifndef _WATER_GENERATOR_H_
#define _WATER_GENERATOR_H_

#include "WaterVertex.h"
#include <vector>

class WaterGenerator {
	const int VERTICES_PER_SQUARE = 3 * 2;
	const int VERTEX_SIZE_BYTES = 8 + 4;

	std::vector<WaterVertex> buffer;
	void storeGridSquare(int col, int row) {
		std::vector<glm::vec2> cornerPos = calculateCornerPositions(col, row);
		storeTriangle(cornerPos, true);
		storeTriangle(cornerPos, false);
	}

	void storeTriangle(std::vector<glm::vec2> cornerPos, bool left) {
		int index0 = left ? 0 : 2;
		glm::vec4 indicators(0.0f);
		buffer.push_back(WaterVertex(cornerPos[index0], indicators));
		int index1 = 1;
		buffer.push_back(WaterVertex(cornerPos[index1], indicators));
		int index2 = left ? 2 : 3;
		buffer.push_back(WaterVertex(cornerPos[index2], indicators));
	}
	
	std::vector<glm::vec2> calculateCornerPositions(int col, int row) {
		std::vector<glm::vec2> vertices;
		vertices.push_back(glm::vec2(col, row));
		vertices.push_back(glm::vec2(col, row+1));
		vertices.push_back(glm::vec2(col+1, row));
		vertices.push_back(glm::vec2(col+1, row+1));
		return vertices;
	}
public:
	WaterGenerator() {};

	std::vector<WaterVertex> createMeshData(int gridCount, int totalVertexCount) {
		// int byteSize = VERTEX_SIZE_BYTES * totalVertexCount;

		for (int row = 0; row < gridCount; ++row) {
			for (int col = 0; col < gridCount; ++col) {
				storeGridSquare(col, row);
			}
		}
		return buffer;
	}
};

#endif
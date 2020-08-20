#ifndef _TERRAIN_INDEX_GENERATOR_H_
#define _TERRAIN_INDEX_GENERATOR_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#elif __linux__
#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <GL/glut.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <vector>

class TerrainIndexGenerator {
	int storeTopSection(std::vector<glm::ivec3> &indices, 
		int rowLength, int vertexLength) {
		int pointer = 0;
		for (int row = 0; row < vertexLength - 3; row++) {
			for (int col = 0; col < vertexLength - 1; col++) {
				int topLeft = (row * rowLength) + (col * 2);
				int topRight = topLeft + 1;
				int bottomLeft = topLeft + rowLength;
				int bottomRight = bottomLeft + 1;
				pointer = storeQuad(topLeft, topRight, bottomLeft, bottomRight, indices, pointer, col % 2 != row % 2);
			}
		}
		return pointer;
	}
	int storeSecondLastLine(std::vector<glm::ivec3> &indices, 
		int pointer, int rowLength, int vertexLength) {
		int row = vertexLength - 3;
		for (int col = 0; col < vertexLength - 1; col++) {
			int topLeft = (row * rowLength) + (col * 2);
			int topRight = topLeft + 1;
			int bottomLeft = (topLeft + rowLength) - col;
			int bottomRight = bottomLeft + 1;
			pointer = storeQuad(topLeft, topRight, bottomLeft, bottomRight, indices, pointer, col % 2 != row % 2);
		}
		return pointer;
	}
	int storeLastLine(std::vector<glm::ivec3> &indices, 
		int pointer, int rowLength, int vertexLength) {
		int row = vertexLength - 2;
		for (int col = 0; col < vertexLength - 1; col++) {
			int topLeft = (row * rowLength) + col;
			int topRight = topLeft + 1;
			int bottomLeft = (topLeft + vertexLength);
			int bottomRight = bottomLeft + 1;
			pointer = storeLastRowQuad(topLeft, topRight, bottomLeft, bottomRight, indices, pointer,
				col % 2 != row % 2);
		}
		return pointer;
	}
	int storeQuad(int topLeft, int topRight, int bottomLeft, int bottomRight, 
		std::vector<glm::ivec3> &indices, int pointer, bool rightHanded) {
		pointer = storeLeftTriangle(topLeft, topRight, bottomLeft, bottomRight, indices, pointer, rightHanded);
		indices.push_back(glm::ivec3(topRight, rightHanded ? topLeft : bottomLeft, bottomRight));
		return pointer + 3;
	}
	int storeLastRowQuad(int topLeft, int topRight, int bottomLeft, int bottomRight, 
		std::vector<glm::ivec3> &indices, int pointer, bool rightHanded) {
		pointer = storeLeftTriangle(topLeft, topRight, bottomLeft, bottomRight, indices, pointer, rightHanded);
		indices.push_back(glm::ivec3(bottomRight, topRight, rightHanded ? topLeft : bottomLeft));
		return pointer + 3;
	}
	int storeLeftTriangle(int topLeft, int topRight, int bottomLeft, int bottomRight, 
		std::vector<glm::ivec3> &indices, int pointer, bool rightHanded) {
		indices.push_back(glm::ivec3(topLeft, bottomLeft, rightHanded ? bottomRight : topRight));
		return pointer + 3;
	}
public:
	TerrainIndexGenerator() {};
	std::vector<glm::ivec3> generateIndexBuffer(int vertexCount) {
		std::vector<glm::ivec3> indices;
		int rowLength = (vertexCount - 1) * 2;
		int pointer = storeTopSection(indices, rowLength, vertexCount);
		pointer = storeSecondLastLine(indices, pointer, rowLength, vertexCount);
		pointer = storeLastLine(indices, pointer, rowLength, vertexCount);
		return indices;
	}
};

#endif
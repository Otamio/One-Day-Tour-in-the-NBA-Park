#ifndef _GRID_SQUARE_H_
#define _GRID_SQUARE_H_

#include "../utilities/color.h"
#include "../scene/EnvConfig.h"
#include "TerrainVertex.h"
#include <vector>
#include <glm/gtx/normal.hpp>

class GridSquare {
protected:
	int row;
	int col;
	int lastIndex;
	std::vector<glm::vec3> positions;
	std::vector<Color> colors;
	glm::vec3 normalLeft;
	glm::vec3 normalRight;
private:
	std::vector<Color> calculateCornerColours(int col, int row, 
		const std::vector<std::vector<Color>> &colors) {
		std::vector<Color> cornerCols;
		cornerCols.push_back(colors[row][col]);
		cornerCols.push_back(colors[row+1][col]);
		cornerCols.push_back(colors[row][col+1]);
		cornerCols.push_back(colors[row+1][col+1]);
		return cornerCols;
	}
	std::vector<glm::vec3> calculateCornerPositions(int col, int row, 
		std::vector<std::vector<GLfloat>> heights) {
		std::vector<glm::vec3> vertices(4, glm::vec3(0.0f));
		vertices[0] = glm::vec3(col * gridOffset, heights[row][col], row * gridOffset);
		vertices[1] = glm::vec3(col * gridOffset, heights[row+1][col], row+1 * gridOffset);
		vertices[2] = glm::vec3(col+1 * gridOffset, heights[row][col+1], row * gridOffset);
		vertices[3] = glm::vec3(col+1 * gridOffset, heights[row+1][col+1], row+1 * gridOffset);
		return vertices;
	}
	void storeTopLeftVertex(std::vector<TerrainVertex> &buffer) {
		buffer.push_back(TerrainVertex(positions[0], normalLeft, colors[0]));
	}
	void storeTopRightVertex(std::vector<TerrainVertex> &buffer) {
		buffer.push_back(TerrainVertex(positions[2], normalRight, colors[2]));
	}
	void storeBottomLeftVertex(std::vector<TerrainVertex> &buffer) {
		buffer.push_back(TerrainVertex(positions[1], normalLeft, colors[1]));
	}
	void storeBottomRightVertex(std::vector<TerrainVertex> &buffer) {
		buffer.push_back(TerrainVertex(positions[3], normalRight, colors[3]));
	}

public:
	GridSquare(int row, int col, std::vector<std::vector<GLfloat>> heights, 
		std::vector<std::vector<Color>> colours) {
		this->positions = calculateCornerPositions(col, row, heights);
		this->colors = calculateCornerColours(col, row, colours);
		this->lastIndex = heights.size() - 2;
		this->row = row;
		this->col = col;
		bool rightHanded = col % 2 != row % 2;
		this->normalLeft = glm::triangleNormal(positions[0], positions[1], positions[rightHanded ? 3 : 2]);
		this->normalRight = glm::triangleNormal(positions[2], positions[rightHanded ? 0 : 1], positions[3]);
	}
	void storeSquareData(std::vector<TerrainVertex> &buffer) {
		storeTopLeftVertex(buffer);
		if (row != lastIndex || col == lastIndex) {
			storeTopRightVertex(buffer);
		}
	}
	void storeBottomRowData(std::vector<TerrainVertex> &buffer) {
		if (col == 0) {
			storeBottomLeftVertex(buffer);
		}
		storeBottomRightVertex(buffer);
	}
};

#endif
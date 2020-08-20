#ifndef _TERRAINGENERATOR_H_
#define _TERRAINGENERATOR_H_

#include "Noise.h"
#include "ColorGenerator.h"
#include "GridSquare.h"
#include "myTerrain.h"
#include "IndexGenerator.h"

class TerrainGenerator {
	Noise myNoise;
	ColorGenerator myColorGenerator;
	std::vector<std::vector<GLfloat>> generateHeights(
		int gridSize, Noise noise) {
		std::vector<std::vector<GLfloat>> heights(gridSize+1,
			std::vector<GLfloat>(gridSize+1, 0.0f));
		for (unsigned z = 0; z < heights.size(); ++z) {
			for (unsigned x = 0; x < heights[z].size(); ++x) {
				heights[z][x] = noise.getNoiseF(x, z);
			}
		}
		return heights;
	}
	int calculateVertexCount(int vertexLength) {
		int bottom2Rows = 2 * vertexLength;
		int remainingRowCount = vertexLength - 2;
		int topCount = remainingRowCount * (vertexLength - 1) * 2;
		return topCount + bottom2Rows;
	}
	std::vector<TerrainVertex> createMeshData(std::vector<std::vector<GLfloat>> heights,
		std::vector<std::vector<Color>> colors, int vertexCount) {
		std::vector<TerrainVertex> buffer;
		std::vector<GridSquare> lastRow;
		for (unsigned row = 0; row < heights.size() - 1; ++row) {
			for (unsigned col = 0; col < heights[row].size() - 1; ++col) {
				GridSquare square(row, col, heights, colors);
				square.storeSquareData(buffer);
				if (row == heights.size() - 2) {
					lastRow.push_back(square);
				}
			}
		}
		for (unsigned i = 0; i < lastRow.size(); ++i) {
			lastRow[i].storeBottomRowData(buffer);
		}
		return buffer;
	}
protected:
	myTerrain *createTerrain(std::vector<std::vector<GLfloat>> heights,
		std::vector<std::vector<Color>> colors) {
		int vertexCount = calculateVertexCount(heights.size());
		std::vector<TerrainVertex> vertices = createMeshData(heights, colors, vertexCount);

		TerrainIndexGenerator indexGen;
		std::vector<glm::ivec3> indices = indexGen.generateIndexBuffer(heights.size());
		return new myTerrain(vertices, indices);
	}
public:
	TerrainGenerator(Noise noise, ColorGenerator colorGen) {
		myNoise = noise;
		myColorGenerator = colorGen;
	}
	myTerrain *generateTerrain(int gridSize) {
		std::vector<std::vector<GLfloat>> heights = generateHeights(gridSize, myNoise);
		std::vector<std::vector<Color>> colors = myColorGenerator.generateColours(heights, myNoise.getAmplitude());
		return createTerrain(heights, colors);
	}
	myTerrain *reseed(int gridSize) {
		myNoise.reseed();
		return generateTerrain(gridSize);
	}
};

#endif
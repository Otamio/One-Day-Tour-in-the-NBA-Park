#ifndef _HEIGHT_GENERATOR_H_
#define _HEIGHT_GENERATOR_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#elif __linux__
#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <GL/glut.h>
#else
#include <GL/glew.h>
#endif

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

/* Place Holder */
#include "TerrainGenerator.h"
#include "IndexGenerator.h"
#include "GridSquare.h"

class HeightGenerator {
	unsigned seed;
	unsigned VexCount;
	int xMult = 49632, zMult = 325176;
	float AMPLITUDE = 6.0f;
	int OCTAVES = 3;
	float ROUGHNESS = 0.3f;
	std::vector<std::vector<GLfloat>> height_map;
	/* Random Numbers */
	GLfloat _rand();
	GLfloat interpolate(float, float, float);
	GLfloat getNoise(unsigned, unsigned);
	GLfloat getSmoothNoise(unsigned, unsigned);
	GLfloat getInterpolatedNoise(float, float);
	GLfloat generateHeight(unsigned, unsigned);
	/* Diamond Square */
	void DiamondSquare();
	void Diamond(int, int, int, int);
	void Square(int, int, int, int);
	void DS_Recurse(int, int, int, int);
public:
	HeightGenerator(unsigned);
	GLfloat getHeight(unsigned, unsigned);
	float getAmplitude() { return AMPLITUDE; }
	void reseed();
};

#endif
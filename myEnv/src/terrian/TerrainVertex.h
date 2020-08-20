#ifndef _TERRAINVERTEX_H_
#define _TERRAINVERTEX_H_

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

#include "../utilities/color.h"

struct TerrainVertex {
	glm::vec3 Pos;
	glm::vec3 Norm;
	Color color;
	TerrainVertex(glm::vec3 a1, glm::vec3 a2, Color a3) {
		Pos = a1;
		Norm = a2;
		color = a3;
	}
};

#endif
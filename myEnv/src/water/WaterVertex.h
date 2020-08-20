#ifndef _WATER_VERTEX_H_
#define _WATER_VERTEX_H_

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
#include <iostream>

struct WaterVertex {
	glm::vec2 corners;
	glm::vec4 indicators;
	WaterVertex(glm::vec2 c, glm::vec4 id) {
		corners = c;
		id = indicators;
	}
	void show() {
		std::cout << corners.x << " " << corners.y << "\n";
		std::cout << indicators[0] << " " <<
			indicators[1] << " " <<
			indicators[2] << " " <<
			indicators[3] << " " << std::endl;
	}
};

#endif

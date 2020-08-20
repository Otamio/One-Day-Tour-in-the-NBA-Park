#ifndef _COLOR_H_
#define _COLOR_H_

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

class Color {
	glm::vec3 col = glm::vec3(0.0f);
	float alpha = 1.0f;
public:
	Color(float r, float g, float b) { col = glm::vec3(r, g, b); }
	Color(glm::vec3 color) { col = color; }
	Color(float r, float g, float b, float a) {
		col = glm::vec3(r, g, b);
		alpha = a;
	}
	Color(float r, float g, float b, bool convert) {
		if (convert) {
			col = glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f);
		}
		else {
			col = glm::vec3(r, g, b);
		}
	}
	Color() {}
	glm::vec3 getColor() { return col; }
	glm::vec4 getColorA() { return glm::vec4(col, alpha); }
	float getR() { return col.x; }
	float getG() { return col.y; }
	float getB() { return col.z; }
	Color duplicate() { return Color(col); }
	static Color interpolateColors(Color, Color, float);
};

#endif

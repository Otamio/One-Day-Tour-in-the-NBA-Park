#ifndef _SIMPLE_SHAPE_H_
#define _SIMPLE_SHAPE_H_

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

#include "Vertices.h"

class simpleShapeE {
protected:
	GLuint vao, vbos[2];
	Vertices vertices;
	std::vector<glm::ivec3> indices;
	void _draw_t();
	void _load_buffer();
public:
	simpleShapeE() {};
	~simpleShapeE() {};
	virtual void draw(GLuint, glm::mat4) = 0;
	virtual void passParam(GLuint) = 0;
	virtual void update() = 0;
};
#endif
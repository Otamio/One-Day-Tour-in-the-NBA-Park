#ifndef _NODE_H_
#define _NODE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#elif __linux__
#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <GL/glut.h>
#else
#include <GL/glew.h>
#endif

#include <glm/mat4x4.hpp>

class Node {
public:
	virtual ~Node() {};
	virtual void draw(GLuint shader, glm::mat4 C) = 0;
	virtual void update() = 0;
	virtual void rotate(float rad, glm::vec3 norm) = 0;
	virtual bool isGeometry() = 0;
};

#endif
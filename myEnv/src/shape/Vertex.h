#ifndef _VERTEX_H_
#define _VERTEX_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#elif __linux__
#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <GL/glut.h>
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

struct Vertex {
  glm::vec3 pos;
  glm::vec3 norm;
  glm::vec2 text;
  Vertex(glm::vec3 p, glm::vec3 nm = glm::vec3(0.0f), 
			glm::vec2 tc = glm::vec2(0.0f)) {
	pos = p;
    norm = nm;
    text = tc;
  }
  void set_posY(GLfloat newY) { pos.y = newY; }
};

#endif

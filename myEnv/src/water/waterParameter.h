#ifndef _WATERPARAMETER_H_
#define _WATERPARAMETER_H_

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

std::vector<glm::vec3> water_vert = std::vector<glm::vec3>{
	glm::vec3(-5.0f,0.0f,-5.0f), glm::vec3(-5.0f,0.0f,5.0f),
	glm::vec3(5.0f,0.0f,5.0f), glm::vec3(5.0f,0.0f,-5.0f)
};

std::vector<glm::ivec3> water_ind = std::vector<glm::ivec3>{
	glm::ivec3(2,1,0), glm::ivec3(3,2,0)
};

#endif

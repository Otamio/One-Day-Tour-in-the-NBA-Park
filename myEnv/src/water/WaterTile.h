#ifndef _WATER_TILE_H_
#define _WATER_TILE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#elif __linux__
#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <GL/glut.h>
#else
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "WaterGenerator.h"
#include "../fbo/FBO.h"
#include "../scene/EnvConfig.h"
#include <iostream>

class WaterTile {
	GLuint vao, vbo;
	int gridLen;
	float height;
	std::vector<WaterVertex> vertices;
	void _load_buffer();
	void _shaderGetParams(GLuint);
	void show_vertex(unsigned);
	void bind_texture(int, int, int, GLuint);
protected:
	glm::mat4 model = glm::scale(glm::mat4(1), glm::vec3(modelRescale));
	GLuint REFLECT_TEX_UNIT = 0;
	GLuint REFRACT_TEX_UNIT = 1;
	GLuint DEPTH_TEX_UNIT = 2;
	float moveFactor = 0;
	float waveSpeed = 0.3f;
	double Lasttime;
public:
	WaterTile(int vc, float ht) {
		gridLen = vc;
		height = ht;
		WaterGenerator generator;
		vertices = generator.createMeshData(vc, 100);
		
		_load_buffer();
	}
	GLuint getVao() { return vao; }
	float getHeight() { return height; }
	int getVertexCount() { return gridLen*gridLen; }
	void draw(GLuint, glm::mat4);
	void draw(GLuint, glm::mat4, int, int, int, GLuint);
	void translate(glm::vec3);
	void updateMoveFactor();
	void update() {
		updateMoveFactor();
	}
};

#endif

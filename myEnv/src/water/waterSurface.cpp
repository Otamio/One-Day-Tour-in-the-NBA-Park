#include "waterSurface.h"
#include "waterParameter.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

waterSurface::waterSurface() {
	vertices.load(water_vert);
	indices = water_ind;
	_load_buffer();
}

waterSurface::~waterSurface() {

}

void waterSurface::draw(GLuint shader, glm::mat4 C) {
	glUseProgram(shader);
	passParam(shader);

	_draw_t();
}

void waterSurface::passParam(GLuint shader) {
	GLuint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

void waterSurface::update() {

}
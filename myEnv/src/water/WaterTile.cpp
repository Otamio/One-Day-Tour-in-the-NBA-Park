#include "WaterTile.h"
#include <glm/gtc/type_ptr.hpp>

void WaterTile::_load_buffer() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(WaterVertex) * vertices.size(),
		vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
		(const void *)offsetof(WaterVertex, indicators));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void WaterTile::_shaderGetParams(GLuint shader) {
	glUseProgram(shader);

	GLuint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	
	GLuint heightLoc = glGetUniformLocation(shader, "height");
	glUniform1f(heightLoc, height);
	GLuint moveFactorLoc = glGetUniformLocation(shader, "moveFactor");
	glUniform1f(moveFactorLoc, moveFactor);

	GLuint ReflectionTexLoc = glGetUniformLocation(shader, "reflectionTexture");
	GLuint RefractionTexLoc = glGetUniformLocation(shader, "refractionTexture");
	GLuint depthTexLoc = glGetUniformLocation(shader, "depthTexture");
	GLuint dudvMapLoc = glGetUniformLocation(shader, "dudvMap");
	glUniform1i(ReflectionTexLoc, REFLECT_TEX_UNIT);
	glUniform1i(RefractionTexLoc, REFRACT_TEX_UNIT);
	glUniform1i(depthTexLoc, DEPTH_TEX_UNIT);
	glUniform1i(dudvMapLoc, DEPTH_TEX_UNIT+1);
}

void WaterTile::show_vertex(unsigned id) {
	vertices[id].show();
}

void WaterTile::draw(GLuint shader, glm::mat4 C) {
	
	_shaderGetParams(shader);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glBindVertexArray(0);
}

void WaterTile::draw(GLuint shader, glm::mat4 C, int reflectionTexture,
	int refractionTexture, int depthTexture, GLuint dudvMap) {
	
	glBindVertexArray(vao);

	glUseProgram(shader);
	_shaderGetParams(shader);

	bind_texture(reflectionTexture, refractionTexture, depthTexture, dudvMap);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glBindVertexArray(0);
}

void WaterTile::bind_texture(int reflectionTexture,
	int refractionTexture, int depthTexture, GLuint dudvMap) {
	glActiveTexture(GL_TEXTURE0 + REFLECT_TEX_UNIT);
	glBindTexture(GL_TEXTURE_2D, reflectionTexture);
	glActiveTexture(GL_TEXTURE0 + REFRACT_TEX_UNIT);
	glBindTexture(GL_TEXTURE_2D, refractionTexture);
	glActiveTexture(GL_TEXTURE0 + DEPTH_TEX_UNIT);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glActiveTexture(GL_TEXTURE0 + DEPTH_TEX_UNIT+1);
	glBindTexture(GL_TEXTURE_2D, dudvMap);
}

void WaterTile::translate(glm::vec3 translation) {
	model = glm::translate(model, translation);
}

void WaterTile::updateMoveFactor() {
	double thisTime = glfwGetTime();
	double elapsed = thisTime - Lasttime;

	moveFactor += waveSpeed * (float) elapsed;
	if (moveFactor > 1.0f)
		moveFactor = 0.0f;

	Lasttime = thisTime;
}
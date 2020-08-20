#ifndef _MY_TERRAIN_H_
#define _MY_TERRAIN_H_

#include "TerrainVertex.h"
#include "../scene/EnvConfig.h"
#include <glm/gtc/type_ptr.hpp>

class myTerrain {
	std::vector<TerrainVertex> vertices;
	std::vector<glm::ivec3> indices;
	GLuint vao, vbos[2];
	void _load_buffer() {
		glGenVertexArrays(1, &vao);
		glGenBuffers(2, vbos);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(TerrainVertex) * vertices.size(),
			vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(),
			indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex),
			(const void *)offsetof(TerrainVertex, Norm));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex),
			(const void *)offsetof(TerrainVertex, color));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	void shaderGetParams(GLuint shader, glm::mat4 C) {
		GLuint modelLoc = glGetUniformLocation(shader, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(C * model));
	}
	void shaderGetParams(GLuint shader, glm::mat4 C, glm::vec4 clipPlane) {
		GLuint modelLoc = glGetUniformLocation(shader, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(C * model));

		GLuint clipPlaneLoc = glGetUniformLocation(shader, "plane");
		glUniform4fv(clipPlaneLoc, 1, glm::value_ptr(clipPlane));
	}
	void _render() {
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, sizeof(glm::ivec3) * indices.size(),
			GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
protected:
	glm::mat4 model = glm::scale(glm::mat4(1), 
		glm::vec3(modelRescale, modelRescale, modelRescale));
public:
	myTerrain(const std::vector<TerrainVertex> &v,
		const std::vector <glm::ivec3> &i) {
		vertices = v;
		indices = i;
		_load_buffer();
	}
	void draw(GLuint shader, glm::mat4 C) {
		glUseProgram(shader);
		shaderGetParams(shader, C);

		_render();
	}
	void draw(GLuint shader, glm::mat4 C, glm::vec4 clipPlane) {
		glUseProgram(shader);
		shaderGetParams(shader, C, clipPlane);

		_render();
	}
};

#endif
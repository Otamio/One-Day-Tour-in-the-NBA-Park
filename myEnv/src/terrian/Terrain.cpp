#include "Terrain.h"
#include <glm/gtc/type_ptr.hpp>
#include <ctime>
#include <soil.h>

Terrain::Terrain(unsigned ct, GLfloat sz=10.0f, GLfloat yval = 0.0f) 
	: Hgenerator(ct) {
	VexCount = ct;
	size = sz;
	y = yval;

	_generate();
	_load_texture();
	_load_buffer();
}

void Terrain::_generate() {
	
	GLfloat hsz = size / 2;

	// define vertex info
	for (unsigned i=0; i!=VexCount; ++i) {
		for (unsigned j=0; j!=VexCount; ++j) {
			vertices.append(
				// vertex position
				Vertex(glm::vec3((GLfloat)j / ((GLfloat)VexCount - 1) * size - hsz,
					Hgenerator.getHeight(i,j), 
					(GLfloat)i / ((GLfloat)VexCount - 1) * size - hsz),
					// vertex normal (pointing up)
					glm::vec3(0, 1.0f, 0),
					// Texture Coordinate
					glm::vec2((GLfloat)j / ((GLfloat)VexCount - 1),
					(GLfloat)i / ((GLfloat)VexCount - 1))
				));
		}
	}

	unsigned pointer = 0;
	// define indices info
	for (unsigned gz = 0; gz < VexCount - 1; gz++) {
		for (unsigned gx = 0; gx < VexCount - 1; gx++) {
			unsigned topLeft = (gz * VexCount) + gx;
			unsigned topRight = topLeft + 1;
			unsigned bottomLeft = ((gz + 1) * VexCount) + gx;
			unsigned bottomRight = bottomLeft + 1;
			indices.push_back(glm::ivec3(topLeft, bottomLeft, topRight));
			indices.push_back(glm::ivec3(topRight, bottomLeft, bottomRight));
		}
	}
	
}

void Terrain::draw(GLuint shader, glm::mat4 C) {
	glUseProgram(shader);
	passParam(shader);

	_draw_t();
}

void Terrain::passParam(GLuint shader) {
	GLuint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

void Terrain::update() {

}

void Terrain::reshape() {
	Hgenerator.reseed();

	// Update the heights
	for (unsigned i = 0; i != VexCount; ++i) {
		for (unsigned j = 0; j != VexCount; ++j) {
			vertices[i*VexCount+j].set_posY(Hgenerator.getHeight(i,j));
		}
	}
	_load_buffer();
}

void Terrain::_load_texture() {
	// Create texture
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Repeat Textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	// User Mipmap
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load texture image
	int width, height, nrChannels;
	unsigned char *data = SOIL_load_image(terrain_grass.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D,
			0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
		);
		SOIL_free_image_data(data);
	}
	else {
		std::cout << "Terrain texture failed to load at path: " << terrain_grass << std::endl;
		SOIL_free_image_data(data);
	}
}
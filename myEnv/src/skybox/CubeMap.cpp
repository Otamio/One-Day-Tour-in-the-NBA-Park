#include "CubeMap.h"
#include "../config.h"
#include <soil.h>

extern Material material1;
extern Material material2;
extern Material material3;
extern Material material4;

CubeMap::CubeMap(const std::string &objFilename, const Material &m,
	glm::vec3 position, float scale, bool oldmode, std::vector<std::string> faces) 
	: D3(objFilename, m, position, scale, oldmode)
{
	texture_faces = faces;
	init_CubeMap();
}

CubeMap::CubeMap(const std::string &objFilename, std::vector<std::string> faces) 
	: D3(objFilename, material4, glm::vec3(0), 1000.0f, false)
{
	texture_faces = faces;
	init_CubeMap();
}

void CubeMap::init_CubeMap() {
	// Bind it to proper texture target
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	
	// Loop and assign values to each face
	int width, height, nrChannels;

	for (GLuint i = 0; i < texture_faces.size(); i++)
	{
		unsigned char *data = SOIL_load_image(texture_faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			SOIL_free_image_data(data);
		} else {
			std::cout << "Cubemap texture failed to load at path: " << texture_faces[i] << std::endl;
			SOIL_free_image_data(data);
		}
	}

	// Specify wrapping and filtering methods
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void CubeMap::draw(GLuint shader, glm::mat4 model) {
	// Bind CubeMap
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	GLuint SkyboxLoc = glGetUniformLocation(shader, "skybox");
	glUniform1i(SkyboxLoc, 0);

	glDepthMask(GL_FALSE);

	glBindVertexArray(D3::getVao());
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	D3::draw(shader, model);

	glDisable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);
}

void CubeMap::update() {
	// Nothing
}
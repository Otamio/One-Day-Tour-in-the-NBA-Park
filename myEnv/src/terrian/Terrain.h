#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "../shape/simpleShape.h"
#include "HeightGenerator.h"
#include "../config.h"

class Terrain : protected simpleShapeE {
protected:
	unsigned VexCount;
	HeightGenerator Hgenerator;
	GLuint textureID;
	GLfloat y, size;
	glm::mat4 model = glm::mat4(1);
	void _generate();
	void _load_texture();
public:
	Terrain(unsigned, GLfloat, GLfloat);
	void draw(GLuint shader, glm::mat4 C) override;
	void passParam(GLuint shader) override;
	void update() override;
	void reshape();
};

#endif

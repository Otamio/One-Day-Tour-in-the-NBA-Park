#ifndef _CUBEMAP_H_
#define _CUBEMAP_H_

#include "../model/D3.h"
#include "../model/Material.h"
#include "../Camera.h"
#include <vector>
#include <string>

class CubeMap : public D3 {
private:
	std::vector<std::string> texture_faces;
	void init_CubeMap();
public:
	GLuint textureID;
	CubeMap(const std::string &objFilename, const Material &m,
		glm::vec3 position, float scale, bool oldmode, std::vector<std::string> fs);
	CubeMap(const std::string &objFilename, std::vector<std::string> fs);
	void draw(GLuint shader, glm::mat4 model);
	void update();
};
#endif
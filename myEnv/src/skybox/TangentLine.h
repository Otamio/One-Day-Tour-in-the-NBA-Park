#include "../scene/Geometry.h"

class TangentLine : public Geometry {
	std::vector<glm::vec3> segmentPoints;
	glm::vec3 pa=glm::vec3(0), pb=glm::vec3(0);
	GLfloat nstep = 150.0f;
	GLfloat stepSize = 1.0f / 150.0f;
	glm::vec3 get_point(GLfloat t);
	void update_points();
public:
	TangentLine(glm::vec3 a, glm::vec3 b);
	void draw(GLuint shader, glm::mat4 C) override;
	glm::vec3 get_first() { return pa; }
	glm::vec3 get_second() { return pb; }
	void update(glm::vec3 new_pa, glm::vec3 new_pb);
};
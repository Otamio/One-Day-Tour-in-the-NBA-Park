#ifndef _WATERSURFACE_H_
#define _WATERSURFACE_H_
#include "../shape/simpleShape.h"

class waterSurface : protected simpleShapeE {
protected:
	glm::mat4 model = glm::mat4(1);
	unsigned gridCount;
	GLfloat size, yval;
public:
	waterSurface();
	waterSurface(unsigned, GLfloat, GLfloat);
	~waterSurface();
	void draw(GLuint, glm::mat4) override;
	void passParam(GLuint) override;
	void update() override;
};




#endif
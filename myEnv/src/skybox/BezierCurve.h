#ifndef _BEZIERCURVE_H_
#define _BEZIERCURVE_H_

#include "../scene/Geometry.h"
#include <vector>

class BezierCurve : public Geometry {
	
	GLfloat nstep = 150.0f;
	GLfloat stepSize = 1.0f / 150.0f;
	std::vector<glm::vec3> controlPoints;
	std::vector<glm::vec3> segmentPoints;
	glm::vec3 a, b, c, d;
	glm::vec3 get_point(GLfloat t);
	void BezierCurve::recalculate();
public:
	BezierCurve(std::vector<glm::vec3> points);

	void draw(GLuint shader, glm::mat4 C) override;
	void show_point(glm::vec3 point);
	void show_point(glm::vec3 point, GLfloat t);
	void updatePoint(glm::vec3 point, unsigned ind);
	void movePoint(glm::vec3 v, unsigned ind);
	void scale(float s);
	void updateParameter();
	unsigned size() { return segmentPoints.size(); }
	glm::vec3 get_point(unsigned i) { return segmentPoints[i]; }
};

#endif
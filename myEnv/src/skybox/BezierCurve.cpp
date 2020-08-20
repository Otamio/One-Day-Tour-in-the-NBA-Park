#include "BezierCurve.h"

BezierCurve::BezierCurve(std::vector<glm::vec3> points) {
	controlPoints = points;
	
	// Use formula provided by project page
	a = -1.0f * points[0] + 3.0f * points[1] - 3.0f * points[2] + points[3];
	b = 3.0f * points[0] - 6.0f * points[1] + 3.0f * points[2];
	c = -3.0f * points[0] + 3.0f * points[1];
	d = points[0];

	// Calculate the points on the segments
	for (GLfloat t = 0.0f; t <= 1.0f; t += stepSize) {
		glm::vec3 point = get_point(t);
#ifdef DEBUG_CURVE
		// show the linesegment points created
		show_point(point);
#endif
		segmentPoints.push_back(point);
	}

	Geometry::set_model(segmentPoints);
}

void BezierCurve::show_point(glm::vec3 point) {
	std::cout << point.x << " "
			  << point.y << " "
		      << point.z << " "
			  << std::endl;
}

void BezierCurve::show_point(glm::vec3 point, GLfloat t) {
	std::cout << t << " ";
	show_point(point);
}

glm::vec3 BezierCurve::get_point(GLfloat t) {
	return t * t * t * a + t * t * b + t * c + d;
}

void BezierCurve::draw(GLuint shader, glm::mat4 C) {
	Geometry::draw(shader, C);
}

void BezierCurve::updatePoint(glm::vec3 point, unsigned ind) {
	controlPoints[ind] = point;
}

void BezierCurve::movePoint(glm::vec3 v, unsigned ind) {
	controlPoints[ind] += v;
}

void BezierCurve::updateParameter() {
	a = -1.0f * controlPoints[0] + 3.0f * controlPoints[1] - 
		3.0f * controlPoints[2] + controlPoints[3];
	
	b = 3.0f * controlPoints[0] - 6.0f * controlPoints[1] + 
		3.0f * controlPoints[2];
	
	c = -3.0f * controlPoints[0] + 3.0f * controlPoints[1];

	d = controlPoints[0];

	recalculate();
}

void BezierCurve::recalculate() {

	segmentPoints.clear();

	for (GLfloat t = 0.0f; t <= 1.0f; t += stepSize) {
		glm::vec3 point = get_point(t);
		segmentPoints.push_back(point);
	}

	Geometry::set_model(segmentPoints);
	Geometry::mycolor_set(glm::vec3(0, 0, 0));
}

void BezierCurve::scale(float s) {
	for (auto &p : controlPoints) {
		p *= s;
	}

	updateParameter();
}
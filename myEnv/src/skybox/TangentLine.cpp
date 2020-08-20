#include "TangentLine.h"

TangentLine::TangentLine(glm::vec3 a, glm::vec3 b) {
	pa = a;
	pb = b;

	update_points();
}

void TangentLine::update_points() {
	segmentPoints.clear();

	// Calculate the points on the segments
	for (GLfloat t = 0.0f; t <= 1.0f; t += stepSize) {
		glm::vec3 point = get_point(t);
		segmentPoints.push_back(point);
	}

	Geometry::set_model(segmentPoints);

	Geometry::mycolor_enable();
	Geometry::mycolor_set(glm::vec3(1.0f, 1.0f, 0.0f));
	Geometry::myalpha_set(0.6f);
	Geometry::enable_draw_line();
}

glm::vec3 TangentLine::get_point(GLfloat t) {
	return t * pa + (1.0f - t) * pb;
}

void TangentLine::draw(GLuint shader, glm::mat4 C) {
	Geometry::draw(shader, C);
}

void TangentLine::update(glm::vec3 new_pa, glm::vec3 new_pb) {
	pa = new_pa;
	pb = new_pb;

	update_points();
}

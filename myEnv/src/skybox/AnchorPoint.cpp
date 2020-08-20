#include "AnchorPoint.h"

AnchorPoint::AnchorPoint(const std::string &filepath, glm::vec3 position,
	float scale, bool old, glm::vec3 color)
	: Geometry(filepath, position, scale, old) {
	Geometry::mycolor_set(color);
	Geometry::myalpha_set(0.8f);
}
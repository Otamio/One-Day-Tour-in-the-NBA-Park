#ifndef _ANCHORPOINT_H_
#define _ANCHORPOINT_H_

#include "../scene/Geometry.h"

class AnchorPoint : public Geometry {
	Geometry *nextNode = nullptr;
	Geometry *prevNode = nullptr;
public:
	AnchorPoint(const std::string &filepath, glm::vec3 position,
		float scale, bool old, glm::vec3 c);
	void set_prev(Geometry *node) override { prevNode = node; }
	void set_next(Geometry *node) override { nextNode = node; }
	Geometry *get_prev() override { return prevNode; }
	Geometry *get_next() override { return nextNode; }
	// void point_move(glm::vec3) override {}
};

#endif
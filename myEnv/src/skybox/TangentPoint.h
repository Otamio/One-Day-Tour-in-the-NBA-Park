#ifndef _TANGENTPOINT_H_
#define _TANGENTPOINT_H_

#include "../scene/Geometry.h"

class TangentPoint : public Geometry {
	Geometry *nextNode = nullptr;
	Geometry *prevNode = nullptr;
public:
	TangentPoint(const std::string &filepath, glm::vec3 position,
		float scale, bool old, glm::vec3 c);
	void set_prev(Geometry *node) override { prevNode = node; }
	void set_next(Geometry *node) override { nextNode = node; }
	Geometry *get_prev() override { return prevNode; }
	Geometry *get_next() override { return nextNode; }
};

#endif

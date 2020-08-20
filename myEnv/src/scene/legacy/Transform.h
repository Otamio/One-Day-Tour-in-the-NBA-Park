#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Node.h"
#include <list>
#include <vector>
#include <glm/gtx/transform.hpp>


class Transform : public Node {
	// Model Coordinate
	glm::mat4 M = glm::mat4(1);
	std::list<Node*> children;
	glm::vec3 position = glm::vec3(0);
	// Body parameters
	bool leftleg = false;
	bool rightleg = false;
	bool leftarm = false;
	bool rightarm = false;
	bool body = false;
	// Culling parameters
	bool cull_node = false;
	bool cull_flag = false;
	unsigned cull_level = 0;
	float cull_dist = 0.0f;
public:
	Transform() {};
	~Transform();
	static std::vector<glm::vec4> cull_planes;
	static bool Transform::hierac;
	static unsigned Transform::hierac_ct;
	static unsigned Transform::hierac_robot_ct;
	glm::mat4 get_model() { return M; }
	void AddChild(Node *child);
	void draw(GLuint shader, glm::mat4 C) override;
	void update() override;
	bool isGeometry() override { return false; }
	void RemoveChild(Node* c) {
		children.remove(c);
	}
	void leg_autorotate();
	void arm_autorotate();
	void arm_rotate(float deg);
	void model_jump();
	void model_rotate();
	void pokeball_run(glm::mat4);
	void pokeball_throw(glm::mat4);
	void pokeball_reset();
	void rotate(float rad, glm::vec3 norm) override {
		for (auto &n : children)
			n->rotate(rad, norm);
		M = glm::rotate(glm::mat4(1), rad, norm) * M;
	}
	void rotateZ(float deg) {
		M = glm::rotate(M, glm::radians(deg), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	void rotateX(float deg) {
		M = glm::rotate(M, glm::radians(deg), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	void rotateY(float deg) {
		M = glm::rotate(M, glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	void move(glm::vec3 direction) {
		position += direction;
		M = glm::translate(M, direction);
	}
	void set_leftleg()  { leftleg = true; }
	void set_rightleg() { rightleg = true; }
	void set_leftarm()  { leftarm = true; }
	void set_rightarm() { rightarm = true; }
	void set_body()		{ body = true; }
	// cull node member functions
	void cull_set_node() { cull_node = true; }
	void cull_enable()	{ cull_flag = true; }
	void cull_disable() { cull_flag = false; }
	void cull_set_sphere();
	void cull_set_level(unsigned l) { cull_level = l; }
	bool cull_in_plane(glm::mat4 C);
};

#endif // !_TRANSFORM_H_


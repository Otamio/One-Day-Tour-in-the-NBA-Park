#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "Transform.h"
#include "Geometry.h"

const float spacing = 2.7f;
const float srobot = 0.5f;

class Robot : public Node {
private:
	// Transforms
	Transform *robot2world;
	Transform *body2robot;
	Transform *head2body;
	Transform *leftarm2body;
	Transform *rightarm2body;
	Transform *leftleg2body;
	Transform *rightleg2body;
	Transform *lefteye2head;
	Transform *righteye2head;
	Transform *leftant2head;
	Transform *rightant2head;
	Transform *pokeball2rightarm;
	// Geometry
	Geometry *head;
	Geometry *body;
	Geometry *limb;
	Geometry *eyeball;
	Geometry *antenna;
	Geometry *pokeball_red;
	// draw flag
	bool draw_flag = true;
#ifdef UNIMODE
	bool cull_flag = false;
	// Culling parameters
	std::vector<glm::vec4> cull_planes;
#endif
public:
	Robot();
	~Robot();

	// time-counter (for the whole object)
	static float Robot::s;
	static int Robot::counter;
	static int Robot::ndraws;
	bool isGeometry() override { return false; }

	virtual void draw(GLuint shader, glm::mat4 C) override {
		if (!draw_flag)	return;
#ifdef UNIMODE
		bool draww = !cull_flag ? true : in_plane(C);

		if (draww) {
#endif
		robot2world->draw(shader, C);
		++Robot::ndraws;
#ifdef UNIMODE
		}
#endif
	};
	virtual void update() override {
		if (counter == 0)  ++counter;

#ifdef ARMY
		// walk
		if (counter <= 40000) {
			if (counter % 400 == 0)
				robot2world->update();
			if (counter >= 0)
				pokeball2rightarm->pokeball_run(rightarm2body->get_model());
	}
		// jump
		else if (counter <= 60000) {
			robot2world->model_jump();
			robot2world->model_rotate();
		}
		// raise right hand
		else if (counter <= 90000) {
			rightarm2body->arm_rotate(-0.002f);
			leftarm2body->arm_rotate(-0.002f);
			if (counter == 90000) pokeball2rightarm->pokeball_reset();
		}
		// throw pokeball
		else {
			rightarm2body->arm_rotate(0.002f);
			leftarm2body->arm_rotate(0.002f);
			pokeball2rightarm->pokeball_throw(rightarm2body->get_model());
		}

		++counter;
		if (counter > 120000) {
			counter = -40000;
			pokeball2rightarm->pokeball_reset();
	}
#else
		// walk
		if (counter <= 4000) {
			if (counter % 40 == 0)
				robot2world->update();
			if (counter >= 0)
				pokeball2rightarm->pokeball_run(rightarm2body->get_model());
		}
		// jump
		else if (counter <= 6000) {
			robot2world->model_jump();
			robot2world->model_rotate();
		}
		// raise right hand
		else if (counter <= 9000) {
			rightarm2body->arm_rotate(-0.04f);
			leftarm2body->arm_rotate(-0.04f);
			if (counter == 9000) pokeball2rightarm->pokeball_reset();
		}
		// throw pokeball
		else {
			rightarm2body->arm_rotate(0.04f);
			leftarm2body->arm_rotate(0.04f);
			pokeball2rightarm->pokeball_throw(rightarm2body->get_model());
		}

		++counter;
		if (counter > 12000) {
			counter = -4000;
			pokeball2rightarm->pokeball_reset();
		}
#endif
	};
	void rotate(float rad, glm::vec3 norm) override {};
	void enable_draw() { draw_flag = true; }
	void disable_draw() { draw_flag = false; }
#ifdef UNIMODE
	void set_cull(const std::vector<glm::vec4> &planes) {
		cull_flag = true;
		cull_planes = planes;
	}
	void unset_cull() { cull_flag = false; }
	bool in_plane(glm::mat4 C) {
		glm::vec4 c = C * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		for (const auto &p : cull_planes) {
			float dist = p.x * c.x + p.y * c.y + p.z * c.z + p.w - (srobot * spacing) / 2;
			if (dist > 0)
				return false;
		}
		return true;
	}
#endif
};

#endif

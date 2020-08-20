#include "Robot.h"
#include "Geometry.h"
#include "Transform.h"
#include <vector>
#include <iostream>

class RobotArmy : public Node {
	// Transforms
	Transform *RobotArmy2world;
	Transform *LB_level_3_RobotArmy;
	Transform *LT_level_3_RobotArmy;
	Transform *RB_level_3_RobotArmy;
	Transform *RT_level_3_RobotArmy;
	Transform *LB_level_2_RobotArmy;
	Transform *LT_level_2_RobotArmy;
	Transform *RB_level_2_RobotArmy;
	Transform *RT_level_2_RobotArmy;
	Transform *LB_level_1_RobotArmy;
	Transform *LT_level_1_RobotArmy;
	Transform *RB_level_1_RobotArmy;
	Transform *RT_level_1_RobotArmy;
	Transform *LB_level_0_RobotArmy;
	Transform *LT_level_0_RobotArmy;
	Transform *RB_level_0_RobotArmy;
	Transform *RT_level_0_RobotArmy;

	// Cull Nodes
	Transform *cull_node_lv4;
	Transform *cull_node_lv3;
	Transform *cull_node_lv2;
	Transform *cull_node_lv1;
	Transform *cull_node_lv0;

	// spheres
	Geometry *sphere2;
	Geometry *sphere4;
	Geometry *sphere8;
	Geometry *sphere16;

	// init functions
	void __init__base();
	void __init__lv4();
	void __init__lv3();
	void __init__lv2();
	void __init__lv1();
	void __init__lv0();
	void __cull__set_sphere();
	void __cull__set_level();

public:
	RobotArmy();
	~RobotArmy();

	static Robot *RobotArmy::robert;
	static Geometry *RobotArmy::sphere;
	static bool RobotArmy::drawRobot;
	static bool RobotArmy::drawSphere;
	static bool RobotArmy::cull;

	virtual void draw(GLuint shader, glm::mat4 C) {
		Robot::ndraws = 0;
		Transform::hierac_ct = 0u;
		Transform::hierac_robot_ct = 0u;
		RobotArmy2world->draw(shader, C);
	};
	virtual void update() {
		RobotArmy2world->update();
	};
	bool isGeometry() override { return false; }
	virtual void rotate(float rad, glm::vec3 norm) {};
#ifdef UNIMODE
	void set_cull(const std::vector<glm::vec4> &planes) {
		RobotArmy::cull = true;
		robert->set_cull(planes);
	}
	void unset_cull() {
		RobotArmy::cull = false;
		robert->unset_cull();
	}
#endif
	bool cull_status() { return RobotArmy::cull;  }
	void draw_robot() { 
		RobotArmy::drawRobot = true; 
		RobotArmy::robert->enable_draw();
	}
	void draw_sphere() { 
		RobotArmy::drawSphere = true;
		RobotArmy::sphere->enable_draw();
	}
	void nodraw_robot() {
		RobotArmy::drawRobot = false;
		RobotArmy::robert->disable_draw();
	}
	void nodraw_sphere() {
		RobotArmy::drawSphere = false;
		RobotArmy::sphere->disable_draw();
	}
	void draw_sphere_level(unsigned level) {
		switch (level) {
		case 3:
			sphere2->enable_draw();
			break;
		case 2:
			sphere4->enable_draw();
			break;
		case 1:
			sphere8->enable_draw();
			break;
		case 0:
			sphere16->enable_draw();
			break;
		}
	}
	void nodraw_sphere_level(unsigned level) {
		switch (level) {
		case 3:
			sphere2->disable_draw();
			break;
		case 2:
			sphere4->disable_draw();
			break;
		case 1:
			sphere8->disable_draw();
			break;
		case 0:
			sphere16->disable_draw();
			break;
		}
	}
	void switch_sphere_level(unsigned level) {
		switch (level) {
		case 3:
			sphere2->switch_draw();
			break;
		case 2:
			sphere4->switch_draw();
			break;
		case 1:
			sphere8->switch_draw();
			break;
		case 0:
			sphere16->switch_draw();
			break;
		}
	}
	void robot_switch() {
		if (RobotArmy::drawRobot) {
			nodraw_robot();
		} else {
			draw_robot();
		}
	}
	void sphere_switch() {
		if (RobotArmy::drawSphere) {
			nodraw_sphere();
		}
		else {
			draw_sphere();
		}
	}
	void cull_update_planes(const std::vector<glm::vec4> &planes) {
		Transform::cull_planes = planes;
	}
	void cull_set(const std::vector<glm::vec4> &planes) {
		cull_node_lv4->cull_enable();
		if (Transform::hierac) {
			cull_node_lv3->cull_enable();
			cull_node_lv2->cull_enable();
			cull_node_lv1->cull_enable();
			cull_node_lv0->cull_enable();
		}
		RobotArmy::cull = true;
		std::cout << "Culling is ON" << std::endl;
	}
	void cull_unset() {
		RobotArmy::cull = false;
		cull_node_lv4->cull_disable();
		cull_node_lv3->cull_disable();
		cull_node_lv2->cull_disable();
		cull_node_lv1->cull_disable();
		cull_node_lv0->cull_disable();
		std::cout << "Culling is OFF" << std::endl;
	}
	void cull_hierac_switch() {
		
		if (Transform::hierac) {
			Transform::hierac = false;
			cull_node_lv3->cull_disable();
			cull_node_lv2->cull_disable();
			cull_node_lv1->cull_disable();
			cull_node_lv0->cull_disable();
			std::cout << "Hierarchical culling is OFF" << std::endl;
		} else {
			Transform::hierac = true;
			cull_node_lv3->cull_enable();
			cull_node_lv2->cull_enable();
			cull_node_lv1->cull_enable();
			cull_node_lv0->cull_enable();
			std::cout << "Hierarchical culling is ON" << std::endl;
		}
	}
};
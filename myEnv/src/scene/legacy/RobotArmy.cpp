#include "../obj/D3.h"
#include "../config.h"
#include "RobotArmy.h"

extern Material material1;
extern Material material2;
extern Material material3;
extern Material material4;

Robot *RobotArmy::robert;
Geometry *RobotArmy::sphere;
bool RobotArmy::drawRobot;
bool RobotArmy::drawSphere;
bool RobotArmy::cull;

RobotArmy::RobotArmy() {
	/* Create the Scene Graph bottom up */
	__init__base();
	__init__lv4();
	__init__lv3();
	__init__lv2();
	__init__lv1();
	__init__lv0();

	// Set cull nodes
	__cull__set_level();
	__cull__set_sphere();
}

void RobotArmy::__init__base() {
	// Create Robot and Sphere Geometry
	RobotArmy::robert = new Robot();
	RobotArmy::sphere = new Geometry(ObjPath_Sphere, material3, glm::vec3(0), spacing * srobot, true);
	RobotArmy::drawRobot = true;
	RobotArmy::cull = false;
	RobotArmy::sphere->disable_draw();
	RobotArmy::sphere->enable_draw_line();
	// RobotArmy::sphere->mycolor_enable();
	// RobotArmy::sphere->mycolor_set(glm::vec3(0.6f,0.8f,0.8f));

	// Hard-code these values
	sphere2 = new Geometry(ObjPath_Sphere, material3, glm::vec3(0), 2.45f * spacing * srobot, true);
	sphere4 = new Geometry(ObjPath_Sphere, material3, glm::vec3(0), 5.30f * spacing * srobot, true);
	sphere8 = new Geometry(ObjPath_Sphere, material3, glm::vec3(0), 11.00f * spacing * srobot, true);
	sphere16 = new Geometry(ObjPath_Sphere, material3, glm::vec3(0), 22.50f * spacing * srobot, true);

	sphere2->disable_draw();
	sphere2->enable_draw_line();
	sphere2->mycolor_enable();
	sphere2->mycolor_set(glm::vec3(0.6f, 0.8f, 0.8f));


	sphere4->disable_draw();
	sphere4->enable_draw_line();
	sphere4->mycolor_enable();
	sphere4->mycolor_set(glm::vec3(0.5f, 0.1f, 0.9f));

	sphere8->disable_draw();
	sphere8->enable_draw_line();
	sphere8->mycolor_enable();
	sphere8->mycolor_set(glm::vec3(0.0f, 0.5f, 0.0f));


	sphere16->disable_draw();
	sphere16->enable_draw_line();
	sphere16->mycolor_enable();
	sphere16->mycolor_set(glm::vec3(0.5f, 0.0f, 0.0f));

	// Create root node
	RobotArmy2world = new Transform();
}

void RobotArmy::__init__lv4() {
	/* Create Level 4 Cull node (Base level) */
	cull_node_lv4 = new Transform();
	cull_node_lv4->cull_set_node();
	cull_node_lv4->AddChild(sphere);
	cull_node_lv4->AddChild(robert);
}

void RobotArmy::__init__lv3() {
	/* Create Level 3 nodes */
	// Create lv3 left bottom node
	LB_level_3_RobotArmy = new Transform();
	LB_level_3_RobotArmy->AddChild(cull_node_lv4);
	// LB_level_3_RobotArmy->AddChild(RobotArmy::robert);
	// LB_level_3_RobotArmy->AddChild(RobotArmy::sphere);
	LB_level_3_RobotArmy->move(glm::vec3(-spacing * srobot, 0.0f, spacing * srobot));

	// Create lv3 left top node
	LT_level_3_RobotArmy = new Transform();
	LT_level_3_RobotArmy->AddChild(cull_node_lv4);
	// LT_level_3_RobotArmy->AddChild(RobotArmy::robert);
	// LT_level_3_RobotArmy->AddChild(RobotArmy::sphere);
	LT_level_3_RobotArmy->move(glm::vec3(-spacing * srobot, 0.0f, -spacing * srobot));

	// Create lv3 right bottom node
	RB_level_3_RobotArmy = new Transform();
	RB_level_3_RobotArmy->AddChild(cull_node_lv4);
	// RB_level_3_RobotArmy->AddChild(RobotArmy::robert);
	// RB_level_3_RobotArmy->AddChild(RobotArmy::sphere);
	RB_level_3_RobotArmy->move(glm::vec3(spacing * srobot, 0.0f, spacing * srobot));

	// Create lv3 right top node
	RT_level_3_RobotArmy = new Transform();
	RT_level_3_RobotArmy->AddChild(cull_node_lv4);
	// RT_level_3_RobotArmy->AddChild(RobotArmy::robert);
	// RT_level_3_RobotArmy->AddChild(RobotArmy::sphere);
	RT_level_3_RobotArmy->move(glm::vec3(spacing * srobot, 0.0f, -spacing * srobot));

	cull_node_lv3 = new Transform();
	cull_node_lv3->cull_set_node();
	cull_node_lv3->AddChild(sphere2);
	cull_node_lv3->AddChild(LB_level_3_RobotArmy);
	cull_node_lv3->AddChild(RB_level_3_RobotArmy);
	cull_node_lv3->AddChild(LT_level_3_RobotArmy);
	cull_node_lv3->AddChild(RT_level_3_RobotArmy);
}

void RobotArmy::__init__lv2() {

	/* Create Level 2 nodes */
	// Create lv2 left bottom node
	LB_level_2_RobotArmy = new Transform();
	LB_level_2_RobotArmy->AddChild(cull_node_lv3);
	// LB_level_2_RobotArmy->AddChild(LB_level_3_RobotArmy);
	// LB_level_2_RobotArmy->AddChild(LT_level_3_RobotArmy);
	// LB_level_2_RobotArmy->AddChild(RB_level_3_RobotArmy);
	// LB_level_2_RobotArmy->AddChild(RT_level_3_RobotArmy);
	LB_level_2_RobotArmy->move(glm::vec3(-2 * spacing * srobot, 0.0f, 2 * spacing * srobot));

	// Create lv2 left top node
	LT_level_2_RobotArmy = new Transform();
	LT_level_2_RobotArmy->AddChild(cull_node_lv3);
	// LT_level_2_RobotArmy->AddChild(LB_level_3_RobotArmy);
	// LT_level_2_RobotArmy->AddChild(LT_level_3_RobotArmy);
	// LT_level_2_RobotArmy->AddChild(RB_level_3_RobotArmy);
	// LT_level_2_RobotArmy->AddChild(RT_level_3_RobotArmy);
	LT_level_2_RobotArmy->move(glm::vec3(-2 * spacing * srobot, 0.0f, -2 * spacing * srobot));

	// Create lv2 right bottom node
	RB_level_2_RobotArmy = new Transform();
	RB_level_2_RobotArmy->AddChild(cull_node_lv3);
	// RB_level_2_RobotArmy->AddChild(LB_level_3_RobotArmy);
	// RB_level_2_RobotArmy->AddChild(LT_level_3_RobotArmy);
	// RB_level_2_RobotArmy->AddChild(RB_level_3_RobotArmy);
	// RB_level_2_RobotArmy->AddChild(RT_level_3_RobotArmy);
	RB_level_2_RobotArmy->move(glm::vec3(2 * spacing * srobot, 0.0f, 2 * spacing * srobot));

	// Create lv2 right top node
	RT_level_2_RobotArmy = new Transform();
	RT_level_2_RobotArmy->AddChild(cull_node_lv3);
	// RT_level_2_RobotArmy->AddChild(LB_level_3_RobotArmy);
	// RT_level_2_RobotArmy->AddChild(LT_level_3_RobotArmy);
	// RT_level_2_RobotArmy->AddChild(RB_level_3_RobotArmy);
	// RT_level_2_RobotArmy->AddChild(RT_level_3_RobotArmy);
	RT_level_2_RobotArmy->move(glm::vec3(2 * spacing * srobot, 0.0f, -2 * spacing * srobot));

	cull_node_lv2 = new Transform();
	cull_node_lv2->cull_set_node();
	cull_node_lv2->AddChild(sphere4);
	cull_node_lv2->AddChild(LB_level_2_RobotArmy);
	cull_node_lv2->AddChild(RB_level_2_RobotArmy);
	cull_node_lv2->AddChild(LT_level_2_RobotArmy);
	cull_node_lv2->AddChild(RT_level_2_RobotArmy);
}

void RobotArmy::__init__lv1() {
	// Create lv1 left bottom node
	LB_level_1_RobotArmy = new Transform();
	LB_level_1_RobotArmy->AddChild(cull_node_lv2);
	// LB_level_1_RobotArmy->AddChild(LB_level_2_RobotArmy);
	// LB_level_1_RobotArmy->AddChild(LT_level_2_RobotArmy);
	// LB_level_1_RobotArmy->AddChild(RB_level_2_RobotArmy);
	// LB_level_1_RobotArmy->AddChild(RT_level_2_RobotArmy);
	LB_level_1_RobotArmy->move(glm::vec3(-4 * spacing * srobot, 0.0f, 4 * spacing * srobot));

	// Create lv1 left bottom node
	LT_level_1_RobotArmy = new Transform();
	LT_level_1_RobotArmy->AddChild(cull_node_lv2);
	// LT_level_1_RobotArmy->AddChild(LB_level_2_RobotArmy);
	// LT_level_1_RobotArmy->AddChild(LT_level_2_RobotArmy);
	// LT_level_1_RobotArmy->AddChild(RB_level_2_RobotArmy);
	// LT_level_1_RobotArmy->AddChild(RT_level_2_RobotArmy);
	LT_level_1_RobotArmy->move(glm::vec3(-4 * spacing * srobot, 0.0f, -4 * spacing * srobot));

	// Create lv1 right bottom node
	RB_level_1_RobotArmy = new Transform();
	RB_level_1_RobotArmy->AddChild(cull_node_lv2);
	// RB_level_1_RobotArmy->AddChild(LB_level_2_RobotArmy);
	// RB_level_1_RobotArmy->AddChild(LT_level_2_RobotArmy);
	// RB_level_1_RobotArmy->AddChild(RB_level_2_RobotArmy);
	// RB_level_1_RobotArmy->AddChild(RT_level_2_RobotArmy);
	RB_level_1_RobotArmy->move(glm::vec3(4 * spacing * srobot, 0.0f, 4 * spacing * srobot));

	// Create lv1 right top node
	RT_level_1_RobotArmy = new Transform();
	RT_level_1_RobotArmy->AddChild(cull_node_lv2);
	// RT_level_1_RobotArmy->AddChild(LB_level_2_RobotArmy);
	// RT_level_1_RobotArmy->AddChild(LT_level_2_RobotArmy);
	// RT_level_1_RobotArmy->AddChild(RB_level_2_RobotArmy);
	// RT_level_1_RobotArmy->AddChild(RT_level_2_RobotArmy);
	RT_level_1_RobotArmy->move(glm::vec3(4 * spacing * srobot, 0.0f, -4 * spacing * srobot));

	cull_node_lv1 = new Transform();
	cull_node_lv1->cull_set_node();
	cull_node_lv1->AddChild(sphere8);
	cull_node_lv1->AddChild(LB_level_1_RobotArmy);
	cull_node_lv1->AddChild(RB_level_1_RobotArmy);
	cull_node_lv1->AddChild(LT_level_1_RobotArmy);
	cull_node_lv1->AddChild(RT_level_1_RobotArmy);
}

void RobotArmy::__init__lv0() {
	// Create lv0 left bottom node
	LB_level_0_RobotArmy = new Transform();
	LB_level_0_RobotArmy->AddChild(cull_node_lv1);
	// LB_level_0_RobotArmy->AddChild(LB_level_1_RobotArmy);
	// LB_level_0_RobotArmy->AddChild(LT_level_1_RobotArmy);
	// LB_level_0_RobotArmy->AddChild(RB_level_1_RobotArmy);
	// LB_level_0_RobotArmy->AddChild(RT_level_1_RobotArmy);
	LB_level_0_RobotArmy->move(glm::vec3(-8 * spacing * srobot, 0.0f, 8 * spacing * srobot));
	// RobotArmy2world->AddChild(LB_level_0_RobotArmy);

	// Create lv0 left top node
	LT_level_0_RobotArmy = new Transform();
	LT_level_0_RobotArmy->AddChild(cull_node_lv1);
	// LT_level_0_RobotArmy->AddChild(LB_level_1_RobotArmy);
	// LT_level_0_RobotArmy->AddChild(LT_level_1_RobotArmy);
	// LT_level_0_RobotArmy->AddChild(RB_level_1_RobotArmy);
	// LT_level_0_RobotArmy->AddChild(RT_level_1_RobotArmy);
	LT_level_0_RobotArmy->move(glm::vec3(-8 * spacing * srobot, 0.0f, -8 * spacing * srobot));
	// RobotArmy2world->AddChild(LT_level_0_RobotArmy);

	// Create lv0 right bottom node
	RB_level_0_RobotArmy = new Transform();
	RB_level_0_RobotArmy->AddChild(cull_node_lv1);
	// RB_level_0_RobotArmy->AddChild(LB_level_1_RobotArmy);
	// RB_level_0_RobotArmy->AddChild(LT_level_1_RobotArmy);
	// RB_level_0_RobotArmy->AddChild(RB_level_1_RobotArmy);
	// RB_level_0_RobotArmy->AddChild(RT_level_1_RobotArmy);
	RB_level_0_RobotArmy->move(glm::vec3(8 * spacing * srobot, 0.0f, 8 * spacing * srobot));
	// RobotArmy2world->AddChild(RB_level_0_RobotArmy);

	// Create lv0 right top node
	RT_level_0_RobotArmy = new Transform();
	RT_level_0_RobotArmy->AddChild(cull_node_lv1);
	// RT_level_0_RobotArmy->AddChild(LB_level_1_RobotArmy);
	// RT_level_0_RobotArmy->AddChild(LT_level_1_RobotArmy);
	// RT_level_0_RobotArmy->AddChild(RB_level_1_RobotArmy);
	// RT_level_0_RobotArmy->AddChild(RT_level_1_RobotArmy);
	RT_level_0_RobotArmy->move(glm::vec3(8 * spacing * srobot, 0.0f, -8 * spacing * srobot));
	// RobotArmy2world->AddChild(RT_level_0_RobotArmy);

	cull_node_lv0 = new Transform();
	cull_node_lv0->cull_set_node();
	cull_node_lv0->AddChild(sphere16);
	cull_node_lv0->AddChild(LB_level_0_RobotArmy);
	cull_node_lv0->AddChild(RB_level_0_RobotArmy);
	cull_node_lv0->AddChild(LT_level_0_RobotArmy);
	cull_node_lv0->AddChild(RT_level_0_RobotArmy);

	RobotArmy2world->AddChild(cull_node_lv0);
}

void RobotArmy::__cull__set_sphere() {

	cull_node_lv0->cull_set_sphere();
	cull_node_lv1->cull_set_sphere();
	cull_node_lv2->cull_set_sphere();
	cull_node_lv3->cull_set_sphere();
	cull_node_lv4->cull_set_sphere();
}

void RobotArmy::__cull__set_level() {
	cull_node_lv0->cull_set_level(0);
	cull_node_lv1->cull_set_level(1);
	cull_node_lv2->cull_set_level(2);
	cull_node_lv3->cull_set_level(3);
	cull_node_lv4->cull_set_level(4);
}

RobotArmy::~RobotArmy() {
	delete RobotArmy2world;
	delete RobotArmy::robert;
	delete RobotArmy::sphere;
}
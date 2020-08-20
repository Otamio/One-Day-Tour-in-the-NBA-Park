#include "Robot.h"
#include "files.h"

extern Material material1;
extern Material material2;
extern Material material3;
extern Material material4;

#ifdef ARMY
int Robot::counter = -20000;
#else
int Robot::counter = -2000;
#endif
float Robot::s = srobot;
int Robot::ndraws = 0;

Robot::Robot() {
	// Load material
	const Material robert_m = material2;

	// Load resources
	head = new Geometry(path_head, robert_m, glm::vec3(0.0f), srobot);
	body = new Geometry(path_body, robert_m, glm::vec3(0.0f), 0.9f*srobot);
	limb = new Geometry(path_limb, robert_m, glm::vec3(0.0f), 1.2f*srobot);
	eyeball = new Geometry(path_eyeball, robert_m, glm::vec3(0.0f), 1.5f*srobot);
	antenna = new Geometry(path_antenna, robert_m, glm::vec3(0.0f), 0.3f*srobot);
	pokeball_red = new Geometry(path_eyeball, robert_m, glm::vec3(0.0f), 3.5f * srobot);
	// pokeball_red->mycolor_enable();
	pokeball_red->mycolor_set(glm::vec3(0.8f, 0.0f, 0.0f));

	// Create root node
	robot2world = new Transform();

	// Create body
	body2robot = new Transform();
	body2robot->AddChild(body);
	robot2world->AddChild(body2robot);
	// Body sits at the center of the robot

	// Create left arm
	leftarm2body = new Transform();
	leftarm2body->AddChild(limb);
	body2robot->AddChild(leftarm2body);
	leftarm2body->move(glm::vec3(1.3f*srobot, 0.0f, 0.0f));
	leftarm2body->rotateZ(40.0f);
	leftarm2body->set_leftarm();

	// Create right arm
	rightarm2body = new Transform();
	rightarm2body->AddChild(limb);
	body2robot->AddChild(rightarm2body);
	rightarm2body->move(glm::vec3(-1.3f*srobot, 0.0f, 0.0f));
	rightarm2body->rotateZ(-40.0f);
	rightarm2body->set_rightarm();

	// pokeball2rightarm
	pokeball2rightarm = new Transform();
	pokeball2rightarm->AddChild(pokeball_red);
	rightarm2body->AddChild(pokeball2rightarm);
	pokeball2rightarm->move(glm::vec3(-0.2f * srobot, -0.45f * srobot, 0.1f * srobot));

	// Create left leg
	leftleg2body = new Transform();
	leftleg2body->AddChild(limb);
	body2robot->AddChild(leftleg2body);
	leftleg2body->move(glm::vec3(0.75f*srobot, -1.40f*srobot, 0.3f*srobot));
	leftleg2body->rotateZ(15.0f);
	leftleg2body->set_leftleg();

	// Create right leg
	rightleg2body = new Transform();
	rightleg2body->AddChild(limb);
	body2robot->AddChild(rightleg2body);
	rightleg2body->move(glm::vec3(-0.75f*srobot, -1.40f*srobot, 0.3f*srobot));
	rightleg2body->rotateZ(-15.0f);
	rightleg2body->set_rightleg();

	// Create Head
	head2body = new Transform();
	head2body->AddChild(head);
	robot2world->AddChild(head2body);
	// Head sits on top of the body
	head2body->move(glm::vec3(0.0f, 1.7f*srobot, 0.0f));

	// Create Left Antenna
	leftant2head = new Transform();
	leftant2head->AddChild(antenna);
	head2body->AddChild(leftant2head);
	// antenna sits on top of the head
	leftant2head->move(glm::vec3(0.4f*srobot, 0.2f*srobot, 0.0f));
	leftant2head->rotateZ(-15.0f);

	// Create Right Antenna
	rightant2head = new Transform();
	rightant2head->AddChild(antenna);
	head2body->AddChild(rightant2head);
	// antenna sits on top of the head
	rightant2head->move(glm::vec3(-0.4f*srobot, 0.2f*srobot, 0.0f));
	rightant2head->rotateZ(15.0f);

	// Create Left Eye
	lefteye2head = new Transform();
	lefteye2head->AddChild(eyeball);
	head2body->AddChild(lefteye2head);
	// eye sits on top of the head
	lefteye2head->move(glm::vec3(0.4f*srobot, 0.035f*srobot, 0.75f*srobot));

	// Create Right Eye
	righteye2head = new Transform();
	righteye2head->AddChild(eyeball);
	head2body->AddChild(righteye2head);
	// eye sits on top of the head
	righteye2head->move(glm::vec3(-0.4f*srobot, 0.035f*srobot, 0.75f*srobot));
}


Robot::~Robot() {
	// This will delete recursively
	delete robot2world;
}
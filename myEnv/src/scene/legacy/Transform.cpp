#include "Transform.h"
#include "Robot.h"
#include <iostream>

std::vector<glm::vec4> Transform::cull_planes;
bool Transform::hierac = false;
unsigned Transform::hierac_ct = 0u;
unsigned Transform::hierac_robot_ct = 0u;

Transform::~Transform() {
	for (Node *n : children)
		n->~Node();
}

void Transform::AddChild(Node *child) {
	children.push_back(child);
}

void Transform::draw(GLuint shader, glm::mat4 C) {
	glm::mat4 MU = C * M;

	// If traverse through a cull node, the check the boundary
	if (cull_flag) {

		// Check if in boundary
		bool draww = cull_in_plane(MU);

		if (!draww) {
			if (cull_level != 4) {
				++Transform::hierac_ct;
				Transform::hierac_robot_ct += glm::pow(4, 4 - cull_level);
			}
			return;
		}
	}

	if (cull_node && cull_level != 4) {
		for (auto &n : children) {
			if (!n->isGeometry() || Transform::hierac)
				n->draw(shader, MU);
		}
		return;
	}

	for (auto &n : children) {
		n->draw(shader, MU);
	}
}

void Transform::update() {
	// std::cout << "autorotate" << std::endl;
	for (auto& n : children)
		n->update();
	if (leftleg || rightleg)
		leg_autorotate();
	else if (leftarm || rightarm)
		arm_autorotate();
}

void Transform::leg_autorotate() {
	// rotation degrees
	float deg = leftleg ? 1.0f : -1.0f;

	// traslate before rotation
	M = glm::translate(M, glm::vec3(0.0f, -position.y, 0.0f));

	if (Robot::counter >= 0)
		rotateX(deg);
	else
		rotateX(-deg);

	M = glm::translate(M, glm::vec3(0.0f, position.y, 0.0f));
}

void Transform::arm_autorotate() {
	// rotation degrees
	float deg = leftarm ? -1.0f : 1.0f;
	float offset = leftarm ? -1.0f * Robot::s : 1.0f * Robot::s;

	// translate before rotation
	M = glm::translate(M, glm::vec3(-offset, 1.0f * Robot::s, 0.0f));

	if (Robot::counter >= 0)
		rotateX(deg);
	else
		rotateX(-deg);

	M = glm::translate(M, glm::vec3(offset, -1.0f * Robot::s, 0.0f));
}

void Transform::arm_rotate(float deg) {
	deg = leftarm ? -1.0f * deg : 1.0f * deg;
	float offset = leftarm ? -1.0f * Robot::s : 1.0f * Robot::s;

	// translate before rotation
	M = glm::translate(M, glm::vec3(-offset, 1.0f * Robot::s, 0.0f));

	rotateX(deg);

	M = glm::translate(M, glm::vec3(offset, -1.0f * Robot::s, 0.0f));
}

void Transform::pokeball_run(glm::mat4 M_out) {
	M = M_out * M;

#ifdef ARMY
	if (Robot::counter < 20000 && Robot::counter >= 0)
		M = glm::translate(M, glm::vec3(0.0f, -0.0001f * srobot, 0));
	else if (Robot::counter < 40000 && Robot::counter >= 20000)
		M = glm::translate(M, glm::vec3(0.0f, 0.0001f * srobot, 0));
#else
	//if (Robot::counter < -2000 && Robot::counter >= -4000)
	//	M = glm::translate(M, glm::vec3(0.0f, -0.001f * srobot, 0));
	if (Robot::counter < 2000 && Robot::counter >= 0)
		M = glm::translate(M, glm::vec3(0.0f, -0.001f * srobot, 0));
	else if (Robot::counter < 4000 && Robot::counter >= 2000)
		M = glm::translate(M, glm::vec3(0.0f, 0.001f * srobot, 0));
	//else if (Robot::counter < 4000 && Robot::counter >= 2000)
	//	M = glm::translate(M, glm::vec3(0.0f, 0.001f * srobot, 0));
#endif
	M = glm::inverse(M_out) * M;
}

void Transform::pokeball_throw(glm::mat4 M_out) {
	M = M_out * M;

	M = glm::translate(M, glm::vec3(-1.0f * Robot::s, 1.0f * Robot::s, 0.0f));
#ifdef ARMY
	rotateX(-0.002f);
#else
	rotateX(-0.04f);
#endif
	M = glm::translate(M, glm::vec3(1.0f * Robot::s, -1.0f * Robot::s, 0.0f));

#ifdef ARMY
	float speed = 0.20f / (Robot::counter - 90000 + 8000);
#else
	float speed = 0.35f / (Robot::counter - 9000 + 800);
#endif
#ifdef ARMY
	M = glm::translate(M, glm::vec3(0.0f, 1 * speed * srobot, 15 * speed * srobot));
#else
	M = glm::translate(M, glm::vec3(0.0f, 1 * speed * srobot, 15 * speed * srobot));
#endif
	M = glm::inverse(M_out) * M;
}

void Transform::pokeball_reset() {
	M = glm::translate(glm::mat4(1),
		glm::vec3(-0.2f * srobot, -0.45f * srobot, 0.1f * srobot));
}

void Transform::model_jump() {
#ifdef ARMY
	if (Robot::counter < 40000)	return;
	float attenuation;

	if (Robot::counter > 40000 && Robot::counter <= 50000) {
		attenuation = 0.01f + 100.0f / (Robot::counter - 40000 + 500) ;
		M = glm::translate(M, glm::vec3(0.0f, 0.0001f * attenuation * Robot::s, 0.0f));
	}
	else if (Robot::counter > 50000 && Robot::counter <= 60000) {
		attenuation = 0.01f + 100.0f / (120001 - Robot::counter + 500);
		M = glm::translate(M, glm::vec3(0.0f, -0.0001f * attenuation * Robot::s, 0.0f));
	}
#else
	if (Robot::counter < 4000)	return;
	float attenuation;

	if (Robot::counter > 4000 && Robot::counter <= 5000) {
		attenuation = 0.01f + 10.0f / (Robot::counter - 4000 + 50);
		M = glm::translate(M, glm::vec3(0.0f, 0.001f * attenuation * Robot::s, 0.0f));
}
	else if (Robot::counter > 5000 && Robot::counter <= 6000) {
		attenuation = 0.01f + 10.0f / (6001 - Robot::counter + 50);
		M = glm::translate(M, glm::vec3(0.0f, -0.001f * attenuation * Robot::s, 0.0f));
	}
#endif
}

void Transform::model_rotate() {
#ifdef ARMY
	if (Robot::counter < 40000)	return;
	const float rotate_deg = 90.0f / 20000.0f;
#else
	if (Robot::counter < 4000)	return;
	const float rotate_deg = 90.0f / 2000.0f;
#endif
	rotateY(rotate_deg);
}

void Transform::cull_set_sphere() {
	const float cscale = 1.0f;

	switch (cull_level) {
	case 4:
		cull_dist = spacing * srobot * cscale;
		break;
	case 3:
		cull_dist = srobot * spacing * 2.45f * cscale;
		break;
	case 2:
		cull_dist = srobot * spacing * 5.30f * cscale;
		break;
	case 1:
		cull_dist = srobot * spacing * 11.00f * cscale;
		break;
	case 0:
		cull_dist = srobot * spacing * 22.50f * cscale;
		break;
	}
}

bool Transform::cull_in_plane(glm::mat4 C) {
	// Get the position of the model center
	glm::vec4 c = C * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	for (const auto &p : Transform::cull_planes) {
		float dist = -1 * (p.x * c.x + p.y * c.y + p.z * c.z + p.w);
		if (dist > cull_dist)
			return false;
	}
	return true;
}
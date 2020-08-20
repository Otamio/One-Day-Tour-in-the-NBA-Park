#include "Roaster.h"
#include "../config.h"

const double g = 9.8;

Roaster::Roaster() {
	// create scene warpping
	skybox = new CubeMap(ObjPath_Cube, hw_blue);
}

Roaster::~Roaster() {
	delete skybox;
}

void Roaster::draw(GLuint shader_texture,
	GLuint shader_environment,
	GLuint shader_color) {

	skybox->draw(shader_texture, glm::mat4(1));
}

void Roaster::update() {

}

glm::vec3 Roaster::sphere_locate(double frame_distance) {

}

void Roaster::sphere_move(double distance) {
	glm::vec3 displacement = sphere_locate(distance);

	sphere->move(displacement);
	height = sphere->get_position().y;
}

void Roaster::sphere_move(glm::vec3 v) {
	sphere->move(v);
	height = sphere->get_position().y;
}

void Roaster::velocity_variable() {
	double v2 = energy - g * height;

	if (v2 < 0)
		velocity = 0.0f;

	velocity = glm::sqrt(2 * v2);
}

void Roaster::friction() {
	if (segment_id == accelarate_id) {
		if (velocity > 50.0f)
			velocity += 0;
		else if (velocity > 35.0f)
			velocity += acceleration_v / (velocity - 33.0f) / (velocity - 33.0f);
		else
			velocity += acceleration_v / 4.0f;
	}
	else {
		if (velocity > 0)
			velocity -= friction_v;
	}

	// std::cout << energy << " " << velocity << std::endl;
	velocity_variable();
}
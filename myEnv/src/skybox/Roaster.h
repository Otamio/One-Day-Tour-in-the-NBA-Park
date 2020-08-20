#ifndef _ROASTER_H_
#define _ROASTER_H_

#include "CubeMap.h"
#include "Track.h"
#include "../config.h"

class Roaster {
	CubeMap *skybox;
	Geometry *sphere;
	Track *track;
	/* Sphere movement parameters */
	bool move_mode = false;
	double Lasttime = 0.0f;
	double delta_time = 0.0f;
	double velocity = 50.0f;
	double frame_distance = 0.0f;
	/* Additional sphere movement parameters */
	bool variable_mode = false;
	double height = 0.0f;
	double energy = 950.0f;

	bool friction_mode = false;
	const double acceleration_v = 50.0f;
	const double friction_v = 0.02f;
	const unsigned accelarate_id = 6u;

	glm::vec3 sphere_position;
	unsigned segment_id = 0;
	unsigned point_id = 0;
	glm::vec3 sphere_locate(double distance);
	void sphere_move(double distance);
	void sphere_move(glm::vec3 v);
	void velocity_reset() { velocity = 50.0f; }
	void energy_reset() { energy = 750.0f; }
	void velocity_variable();
	void friction();
public:
	Roaster();
	~Roaster();
	void draw(GLuint shader_texture,
		GLuint shader_environment,
		GLuint shader_color);
	void update();
	void cp_next() { track->highlight_next(); }
	void cp_prev() { track->highlight_prev(); }
	void cp_update(glm::vec3 p) { 
		track->highlight_move(p);
		track->sphere_update(sphere, segment_id, point_id);
		height = sphere->get_position().y;
	}
	void move_set() { move_mode = true; }
	void move_unset() { move_mode = false; }
	void move_switch() { 
		move_mode = !move_mode; 
		if (move_mode)
			std::cout << "Sphere movement ENABLED" << std::endl;
		else
			std::cout << "Sphere movement DISABLED" << std::endl;
	}
	void move_variable_switch() { 
		variable_mode = !variable_mode; 
		if (variable_mode)
			std::cout << "Variable velocity ENABLED" << std::endl;
		else
			std::cout << "Variable velocity DISABLED" << std::endl;
	}
	void move_friction_switch() {
		if (!variable_mode) {
			std::cout << "To enable friction simulation, please turn on variable velocity mode first"
				<< std::endl;
			return;
		}

		friction_mode = !friction_mode;
		if (friction_mode) {
			std::cout << "Friction Simulation ENABLED" << std::endl;
			track->acceleration(accelarate_id);
		}
		else {
			track->deacceleration(accelarate_id);
			std::cout << "Friction Simulation DISABLED" << std::endl;
		}
	}
};

#endif
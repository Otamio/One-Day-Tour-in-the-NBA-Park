#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include "../Camera.h"
#include "Node.h"
#include "../obj/D3.h"

class Geometry : public Node {
	glm::mat4 view = glm::mat4(1);
	D3 *model;
	GLuint this_shader = 0;
	// draw flag
	bool draw_flag = true;
	bool draw_line = false;
public:
	Geometry(const std::string &filepath) { 
		model = new D3(filepath);
		this_shader = 0;
	};
	Geometry() {
		model = nullptr;
		this_shader = 0;
	}
	Geometry(const std::vector<glm::vec3> segmentPoints) {
		model = new D3(segmentPoints);
		this_shader = 0;
	}
	Geometry(const std::string &filepath, glm::vec3 position, float scale, bool old = false) {
		model = new D3(filepath, Material(), position, scale, old);
		this_shader = 0;
	}
	Geometry(const std::string &filepath, const Material &m, glm::vec3 position, float scale, bool old=false) {
		model = new D3(filepath, m, position, scale, old);
		this_shader = 0;
	};
	~Geometry() {
		delete model;
	};
	void set_model(const std::vector<glm::vec3> segmentPoints) {
		delete model;
		model = new D3(segmentPoints);
	}
	void draw(GLuint shader, glm::mat4 C) override {
		if (draw_flag)
			if (draw_line)
				model->draw_line(shader, C);
			else
				model->draw(shader, C);
	};
	glm::vec3 get_position() {
		return model->getPosition();
	}
	void draw_with_environment(GLuint shader, glm::mat4 C, Camera *myCamera) {
		glUseProgram(shader);
		GLuint viewPosLoc = glGetUniformLocation(shader, "viewPos");
		glUniform3fv(viewPosLoc, 1, glm::value_ptr(myCamera->get_eye()));
		GLuint projectionLoc = glGetUniformLocation(shader, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(myCamera->get_projection()));
		GLuint viewLoc = glGetUniformLocation(shader, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(myCamera->get_view()));
		
		model->draw(shader, C);
	}
	void rotate(float rad, glm::vec3 norm) override {};
	void update() {
		model->update();
	};
	void enable_draw() { draw_flag = true; }
	void disable_draw() { draw_flag = false; }
	bool status_draw() { return draw_flag; }
	void switch_draw() {
		if (draw_flag)	disable_draw();
		else	enable_draw();
	}
	void enable_draw_line() { draw_line = true;  }
	void disable_draw_line() { draw_line = false; }
	bool isGeometry() override { return true; }
	/* Transformation */
	void move(glm::vec3 v) { model->move(v); }
	/* Set color parameters */
	void mycolor_enable() { model->mycolor_enable(); }
	void mycolor_disable() { model->mycolor_disable(); }
	void mycolor_set(glm::vec3 new_color) { model->mycolor_set(new_color); }
	void myalpha_set(GLfloat new_alpha) { model->myalpha_set(new_alpha); }
	/* Linkedlist implementation */
	virtual void set_prev(Geometry *node) { }
	virtual void set_next(Geometry *node) { }
	virtual Geometry *get_prev() { return nullptr; }
	virtual Geometry *get_next() { return nullptr; }
	virtual void point_move(glm::vec3 v) { model->move(v); }
};

#endif
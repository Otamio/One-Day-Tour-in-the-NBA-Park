#ifndef _D3_H_
#define _D3_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#elif __linux__
#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <GL/glut.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <iostream>

#include "debug.h"
#include "Data.h"
#include "Material.h"
#include "../Camera.h"

/* The class D3 describes a 3D object
 */
class D3 {
protected:
  glm::mat4 obj_coord; // obj coordinate
  glm::vec3 p_position = glm::vec3(0.0f);
private:
  /* Object Data */
  Data *data;
  glm::vec3 color;
  GLfloat alpha = 1.0f;
  GLfloat usecolor = 0.0f;
  /* OpenGL dependencies */
  GLuint vao, vbos[2];
  /* Tools : Load Data and initialize object */
  void __Load_Buffer();
  void __Load_Segment();
public:
  // D3(const std::string &objFilename);
  D3(const std::vector<glm::vec3> segmentPoints);
  D3(const std::string &objFilename, const Material &m);
  D3(const std::string &objFilename, const Material &m, glm::vec3 position, float scale, bool);
  ~D3();
  /* Get Parameters */
  inline glm::mat4 getObjCoord() { return obj_coord; }
  inline glm::vec3 getPosition() { return p_position;  }
  inline glm::vec3 getColor() { return color; }
  inline GLuint getVao() { return vao; }
  /* Utilities */
  void spin(float deg);
  void draw();
  void draw(GLuint shader, glm::mat4 C);
  void draw(GLuint shader, glm::mat4 C, glm::vec4 clipPlane);
  void draw_line(GLuint shader, glm::mat4 C);
  void update();
  void rotate(float rad, glm::vec3 a);
  void rotate_point(float rad, glm::vec3 a);
  void move(glm::vec3 direction) {
	  p_position += direction;
	  obj_coord = glm::translate(obj_coord, direction);
  }
  void show_position() {
	  std::cout << p_position.x << " " <<
		  p_position.y << " " <<
		  p_position.z << " " << std::endl;
  }
  void scale(float s);
  void scaleUp();
  void scaleDown();
  /* Materials */
  Material material;
  /* Data Manipulation */
  void set_center(glm::vec3 center) { data->reposition(center); }
  /* Set color parameters */
  void mycolor_enable() { usecolor = 1.0f; }
  void mycolor_disable() { usecolor = 0.0f; }
  void mycolor_set(glm::vec3 new_color) { color = new_color; }
  void myalpha_set(GLfloat new_alpha) { alpha = new_alpha; }
};

#endif

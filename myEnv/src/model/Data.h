#ifndef _DATA_H_
#define _DATA_H_

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
#include "../shape/Vertex.h"
#include "../config.h"

class Data {
  friend class D3;
  /* Object Data */
  bool segment = false;
  std::vector<glm::vec3> segments;
  std::vector<Vertex> vertices;
  std::vector<glm::ivec3> indices;
  /* Object Statistics */
  float x_max=0, x_min=0, y_max=0, y_min=0, z_max=0, z_min=0;
  /* Loading Data, Positioning and Scaling */
  void __Load_Model(const std::string &objFilename, bool);
  void __Center();
  void __Scale();
  void __Scale(float);
public:
  Data(const std::string &objFilename);
  Data(const std::string &objFilename, float, bool);
  Data(const std::vector<glm::vec3> segmentPoints);
  void set_center(glm::vec3 center);
  void reposition(glm::vec3 position) {
	  for (auto &v : vertices)
		  v.pos += position;
  }
};

#endif

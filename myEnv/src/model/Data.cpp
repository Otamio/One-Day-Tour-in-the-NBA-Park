#include <iostream>
#include <fstream>
#include <sstream>
#include "Data.h"

#define _FACE_SKIP 2

Data::Data(const std::string &objFilename) {
  // Load points, point norms, and faces
	__Load_Model(objFilename, true);

  // Rescale object
  __Center();
  __Scale();
}

Data::Data(const std::string& objFilename, float scale, bool old=false) {
	// Load points, point norms, and faces
	__Load_Model(objFilename, old);
	
	// Rescale object
	__Center();
	__Scale(scale);
}

Data::Data(const std::vector<glm::vec3> segmentPoints) {
	segment = true;
	segments = segmentPoints;
}

void Data::__Load_Model(const std::string &objFilename, bool old=false) {

  std::vector<glm::vec3> points;
  std::vector<glm::vec3> pnorms;
  std::vector<glm::vec2> textcoordinates;

  std::ifstream objFile(objFilename); // Create a file handler

	if (objFile.is_open()) {
		std::string line;

		while (std::getline(objFile, line)) {
			// Turn the line into a string stream for processing.
			std::stringstream ss;
			ss << line;

			// Read the first word of the line.
			std::string label;
			ss >> label;

			if (label == "v") { // Vertex data
				glm::vec3 point;
				ss >> point.x >> point.y >> point.z;

				points.push_back(point);
			} else if (label == "vn") { // Vertex norm
				glm::vec3 pnorm;
				ss >> pnorm.x >> pnorm.y >> pnorm.z;

				pnorms.push_back(pnorm);
            } else if (label == "vt") { // Text coordinates
				glm::vec2 textc;
				ss >> textc.x >> textc.y;
            } else if (label == "f") { // Faces
				int x, y, z, temp;
				if (old) {
					ss >> x;
					ss.ignore(_FACE_SKIP);
					ss >> x;
					ss >> y;
					ss.ignore(_FACE_SKIP);
					ss >> y;
					ss >> z;
				}
				else {
					ss >> x;
					ss.ignore(1);
					ss >> temp;
					ss.ignore(1);
					ss >> temp;

					ss >> y;
					ss.ignore(1);
					ss >> temp;
					ss.ignore(1);
					ss >> temp;

					ss >> z;
					ss.ignore(1);
					ss >> temp;
					ss.ignore(1);
					ss >> temp;
				}

				indices.push_back(glm::ivec3(x - 1, y - 1, z - 1));
			}
		}
	} else
		std::cerr << "Can't open the file " << objFilename << std::endl;

	objFile.close();

	if (textcoordinates.size() == 0)
		for (unsigned i = 0; i != points.size(); ++i) {
			vertices.push_back(Vertex(points[i], pnorms[i]));
		}
	else
		for (unsigned i = 0; i != points.size(); ++i) {
			vertices.push_back(Vertex(points[i], pnorms[i], textcoordinates[i]));
		}
}

void Data::__Center() {

  // Iterate through all the points to calculate min and max of each coordinate
  for (const auto &v : vertices) {
    auto p = v.pos;

    if (p.x < x_min)			x_min = p.x;
    else if (p.x > x_max) x_max = p.x;

    if (p.y < y_min)  		y_min = p.y;
    else if (p.y > y_max) y_max = p.y;

    if (p.z < z_min)			z_min = p.z;
    else if (p.z > z_max)	z_max = p.z;
  }
  float x_mid = (x_max+x_min)/2.0f, y_mid = (y_max+y_min)/2.0f,
        z_mid = (z_max+z_min)/2.0f;

  // Iterate through all the points to adjust their coordinates
  for (auto &v : vertices) {
    v.pos.x -= x_mid;
    v.pos.y -= y_mid;
    v.pos.z -= z_mid;
  }

  // update extreme values
  x_max -= x_mid;
  x_min -= x_mid;
  y_max -= y_mid;
  y_min -= y_mid;
  z_max -= z_mid;
  z_min -= z_mid;
}

void Data::__Scale() {

  const float limit = 10.0f;
  float dist_max = 0.0f;
  // Iterate to calculate maximum distance
  for (const auto &v : vertices) {
    auto p = v.pos;

    float dist = sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
    if (dist > dist_max)
      dist_max = dist;
  }

  // filter out 0 values
  if (!dist_max)	return;

  const float alpha = limit / dist_max;
  // Iterate to update all points
  for (auto &v : vertices) {
    v.pos.x *= alpha;
    v.pos.y *= alpha;
    v.pos.z *= alpha;
  }

  x_max *= alpha;
  x_min *= alpha;
  y_max *= alpha;
  y_min *= alpha;
  z_max *= alpha;
  z_min *= alpha;
}

void Data::__Scale(float scale) {
	for (auto& v : vertices) {
		v.pos *= glm::vec3(scale);
	}
}

void Data::set_center(glm::vec3 center) {
	for (auto &v : vertices) {
		v.pos += center;
	}
	x_max += center.x;
	x_min += center.x;
	y_max += center.y;
	y_min += center.y;
	z_max += center.z;
	z_min += center.z;
}
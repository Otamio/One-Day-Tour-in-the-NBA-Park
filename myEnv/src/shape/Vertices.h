#ifndef _VERTICES_H_
#define _VERTICES_H_

#include "Vertex.h"
#include <vector>
#include <iostream>

class Vertices {
	friend class simpleShapeE;
	std::vector<Vertex> myData;
public:
	Vertices() {};
	decltype(myData.data()) data() { return myData.data(); }
	decltype(myData.size()) size() { return myData.size(); }
	void load(std::vector<glm::vec3> p) {
		for (const auto &pt : p)
			myData.push_back(Vertex(pt));
	};
	void load(std::vector<glm::vec3> p,
		std::vector<glm::vec3> n) {
		for (unsigned i = 0; i != p.size(); ++i)
			myData.push_back(Vertex(p[i], n[i]));
	};
	void load(std::vector<glm::vec3> p,
		std::vector<glm::vec3> n,
		std::vector<glm::ivec3> tc) {
		for (unsigned i=0; i!=p.size(); ++i)
			myData.push_back(Vertex(p[i],n[i],tc[i]));
	};
	void append(Vertex v) { myData.push_back(v); }
	void show_element(unsigned i) {
		if (i >= myData.size()) return;

		std::cout << "Position: " << myData[i].pos.x << " "
			<< myData[i].pos.y << " "
			<< myData[i].pos.z << std::endl;
		std::cout << "Normal: " << myData[i].norm.x << " "
			<< myData[i].norm.y << " "
			<< myData[i].norm.z << std::endl;
		std::cout << "Texture Coordinate: " << myData[i].text.x << " "
			<< myData[i].text.y << std::endl;
	}
	Vertex &operator[] (unsigned i) {
		return myData[i];
	}
};

#endif
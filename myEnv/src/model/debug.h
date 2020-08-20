#ifndef _DEBUG_H_
#define _DEBUG_H_

const std::vector<glm::vec3> cube_vertices {
	glm::vec3(-1, 1, 1),
	glm::vec3(-1, -1, 1),
	glm::vec3(1, -1, 1),
	glm::vec3(1, 1, 1),
	glm::vec3(-1, 1, -1),
	glm::vec3(-1, -1, -1),
	glm::vec3(1, -1, -1),
	glm::vec3(1, 1, -1)
};

const std::vector<glm::ivec3> cube_indices {
  // Front face.
  glm::ivec3(0, 1, 2),
  glm::ivec3(2, 3, 0),
  // Back face.
  glm::ivec3(7, 6, 5),
  glm::ivec3(5, 4, 7),
  // Right face.
  glm::ivec3(3, 2, 6),
  glm::ivec3(6, 7, 3),
  // Left face.
  glm::ivec3(4, 5, 1),
  glm::ivec3(1, 0, 4),
  // Top face.
  glm::ivec3(4, 0, 3),
  glm::ivec3(3, 7, 4),
  // Bottom face.
  glm::ivec3(1, 5, 6),
  glm::ivec3(6, 2, 1),
};

#endif

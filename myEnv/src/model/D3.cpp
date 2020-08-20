#include "D3.h"
#include "../shape/Vertex.h"
#include "../../../core/Window.h"

#define _SCALE_UP 1.04f
#define _SCALE_DOWN 0.96f

D3::D3(const std::vector<glm::vec3> segmentPoints) {

	data = new Data(segmentPoints);

	// Set model statistics
	obj_coord = glm::mat4(1); // Initialize object coordinate as identity matrix
	color = glm::vec3(1.0f, 1.0f, 1.0f); // Set color

	// Load VAO, VBOs, and EVO
	__Load_Segment();
	material = Material();
}

D3::D3(const std::string &objFilename, 
	const Material &m=Material()) {

	data = new Data(objFilename);

	// Set model statistics
	obj_coord = glm::mat4(1); // Set obj_coordinate as identity
	color = glm::vec3(0.8f, 0.8f, 0.8f); // Set color

	// Load VAO, VBOs, and EVO
	__Load_Buffer();
	material = Material(m);
}

D3::D3(const std::string &objFilename, const Material& m, glm::vec3 position, float s, bool old=false) {
	
	data = new Data(objFilename, s, old);
	// Set model statistics
#ifndef P4
	obj_coord = glm::mat4(1); // Set obj_coordinate as identity
#else
	obj_coord = glm::translate(glm::mat4(1), position);
	p_position = position;
#endif
	color = glm::vec3(0.8f, 0.8f, 0.8f); // Set color

	// Load VAO, VBOs, and EVO
	__Load_Buffer();
	material = Material(m);
}

D3::~D3() {
	// Delete the EBO, EBO, and the VAO
	glDeleteBuffers(2, vbos);
	glDeleteVertexArrays(1, &vao);

	delete data;
}

void D3::draw() {
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, sizeof(glm::ivec3) * data->vertices.size(),
                  GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void D3::draw(GLuint shader, glm::mat4 C) {
	
	glUseProgram(shader);

	material.MActivate(shader);

	glm::mat4 model = C * obj_coord;

	GLuint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	GLuint usecolorLoc = glGetUniformLocation(shader, "usecolor");
	glUniform1f(usecolorLoc, usecolor);

	GLuint alphaLoc = glGetUniformLocation(shader, "alpha");
	glUniform1f(alphaLoc, alpha);
	
	GLuint colorLoc = glGetUniformLocation(shader, "color");
	glUniform3fv(colorLoc, 1, glm::value_ptr(color));

	draw();
}

void D3::draw(GLuint shader, glm::mat4 C, glm::vec4 clipPlane) {
	glUseProgram(shader);
	GLuint clipPlaneLoc = glGetUniformLocation(shader, "plane");
	glUniform4fv(clipPlaneLoc, 1, glm::value_ptr(clipPlane));

	draw(shader, C);
}

void D3::draw_line(GLuint shader, glm::mat4 C) {
	
	glUseProgram(shader);

	material.MActivate(shader);

	glm::mat4 model = C * obj_coord;

	GLuint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	GLuint usecolorLoc = glGetUniformLocation(shader, "usecolor");
	glUniform1f(usecolorLoc, usecolor);
	GLuint colorLoc = glGetUniformLocation(shader, "color");
	glUniform3fv(colorLoc, 1, glm::value_ptr(color));
	GLuint alphaLoc = glGetUniformLocation(shader, "alpha");
	glUniform1f(alphaLoc, alpha);

	glBindVertexArray(vao);

	if (!data->segment) {
		glDrawElements(GL_LINE_STRIP, sizeof(glm::ivec3) * data->indices.size(),
			GL_UNSIGNED_INT, 0);
	} else {
		glLineWidth(3.0f);
		glDrawArrays(GL_LINE_STRIP, 0, data->segments.size());
		// std::cout << "size " << data.segments.size() << std::endl;
	}
	// std::cout << color.x << " " << color.y << " " << color.z << std::endl;

	glBindVertexArray(0);
}

void D3::spin(float deg) {
	obj_coord = glm::rotate(obj_coord, glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

void D3::update() {
  // spin(0.1f);
}

void D3::rotate(float rad, glm::vec3 a) {
	obj_coord = glm::translate(obj_coord, -1.0f * p_position);
	glm::mat4 temp = glm::mat4(1);
	obj_coord = glm::rotate(temp, rad, a) * obj_coord;
	obj_coord = glm::translate(obj_coord, p_position);
}

void D3::rotate_point(float rad, glm::vec3 a) {
	glm::vec3 new_position = glm::rotate(rad, a) * glm::vec4(p_position, 1.0f);
	if (glm::all(glm::isnan(new_position)))
		return;
	move(new_position - p_position);
}

void D3::__Load_Buffer() {
  // Generate Buffers (VAO, VBOs, and EBO)
  glGenVertexArrays(1, &vao);
  glGenBuffers(2, vbos);

  // Bind the vertex to vao
  glBindVertexArray(vao);

  // Bind to the position VBO to the array buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
  // Pass position data
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * data->vertices.size(),
			data->vertices.data(), GL_STATIC_DRAW);

  // Bind ebo to vao
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
  // pass indices data to Element array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * data->indices.size(),
      data->indices.data(), GL_STATIC_DRAW);

  // Create channel between vbo and shader
  glEnableVertexAttribArray(0);
  // Instruct shader to read vob
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);

  // Enable Vertex Normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
	(const void *) offsetof(Vertex, norm));

  // Enable Vertex textCoordinates
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
	  (const void *)offsetof(Vertex, text));

  // Unbind from the Ebo
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Unbind from the VAO.
}

void D3::__Load_Segment() {

	// Generate Buffers (VAO, VBOs, and EBO)
	glGenVertexArrays(1, &vao);
	glGenBuffers(2, vbos);

	// Bind the vertex to vao
	glBindVertexArray(vao);

	// Bind to the position VBO to the array buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	// Pass position data
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * data->segments.size(),
		data->segments.data(), GL_STATIC_DRAW);

	// Create channel between vbo and shader
	glEnableVertexAttribArray(0);
	// Instruct shader to read vob
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind from the Ebo
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void D3::scale(float s) {
	obj_coord = glm::scale(obj_coord, glm::vec3(s));
}

void D3::scaleUp() {
	obj_coord = glm::translate(obj_coord, -1.0f * p_position);
	obj_coord = glm::scale(obj_coord, glm::vec3(_SCALE_UP));
	p_position *= _SCALE_UP;
	obj_coord = glm::translate(obj_coord, p_position);
}

void D3::scaleDown() {
	obj_coord = glm::translate(obj_coord, -1.0f * p_position);
	obj_coord = glm::scale(obj_coord, glm::vec3(_SCALE_DOWN));
	p_position *= _SCALE_DOWN;
	obj_coord = glm::translate(obj_coord, p_position);
}

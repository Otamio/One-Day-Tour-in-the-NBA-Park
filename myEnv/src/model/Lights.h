#ifndef _LIGHTS_H_
#define _LIGHTS_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#elif __linux__
#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <GL/glut.h>
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>

#define _LSCALE_UP 1.04
#define _LSCALE_DOWN 0.96

/* This class controls light and its statistics
 */
struct Light {
  unsigned light_id;
  glm::vec3 LPosition;
  glm::vec3 LColor;
  glm::vec3 LAmbient;
  glm::vec3 LDiffuse;
  glm::vec3 LSpecular;
  Light(unsigned id, glm::vec3 position, glm::vec3 color) {
	  light_id = id;
	  LPosition = position;
	  LColor = color;
	  LAmbient = glm::vec3(0.0f);
	  LDiffuse = glm::vec3(0.0f);
	  LSpecular = glm::vec3(0.0f);
  }
  void LActivate(GLuint shader) {
    GLuint lightPositionLoc = glGetUniformLocation(shader, "LPosition");
    GLuint lightColorLoc    = glGetUniformLocation(shader, "LColor");
    GLuint lightAmbientLoc  = glGetUniformLocation(shader, "LAmbient");
    GLuint lightDiffuseLoc  = glGetUniformLocation(shader, "LDiffuse");
    GLuint lightSpecularLoc = glGetUniformLocation(shader, "LSpecular");

    glUniform3fv(lightPositionLoc, 1, glm::value_ptr(LPosition));
    glUniform3fv(lightColorLoc,    1, glm::value_ptr(LColor));
    glUniform3fv(lightAmbientLoc,  1, glm::value_ptr(LAmbient));
    glUniform3fv(lightDiffuseLoc,  1, glm::value_ptr(LDiffuse));
    glUniform3fv(lightSpecularLoc, 1, glm::value_ptr(LSpecular));
  }
};

/* This class controls PointLights
 */
struct PointLight {
	Light light;
	D3 model;
	GLuint Sphong = 0;
  PointLight(const std::string &objPath, glm::vec3 position, glm::vec3 color, Material material) 
	: light(1u, position, color), model(objPath, material, position, 0.4f, true) {
	  model.move(position);
  }
  void draw(GLuint shader) {
    glUseProgram(shader);
    PLActivate(shader);
	model.draw(shader);
	Sphong = shader;
  }
  void rotate(float rad, glm::vec3 a) {
	  model.rotate_point(rad, a);
	  light.LPosition = model.getPosition();
	  PLActivate(Sphong);
  }
  void scaleUp() {
	  model.scaleUp();
	  light.LPosition *= _LSCALE_UP;
	  PLActivate(Sphong);
  }
  void scaleDown() {
	  model.scaleDown();
	  light.LPosition *= _LSCALE_DOWN;
	  PLActivate(Sphong);
  }
  void show_position() {
	  std::cout << light.LPosition.x << " " <<  
				   light.LPosition.y << " " << 
		           light.LPosition.z << " ";
	  model.show_position();
  }
  void PLActivate(GLuint shader) {
	  GLuint lightPositionLoc = glGetUniformLocation(shader, "PLPosition");
	  GLuint lightColorLoc = glGetUniformLocation(shader, "PLColor");
	  GLuint lightAmbientLoc = glGetUniformLocation(shader, "PLAmbient");
	  GLuint lightDiffuseLoc = glGetUniformLocation(shader, "PLDiffuse");
	  GLuint lightSpecularLoc = glGetUniformLocation(shader, "PLSpecular");

	  glUniform3fv(lightPositionLoc, 1, glm::value_ptr(light.LPosition));
	  glUniform3fv(lightColorLoc, 1, glm::value_ptr(light.LColor));
	  glUniform3fv(lightAmbientLoc, 1, glm::value_ptr(light.LAmbient));
	  glUniform3fv(lightDiffuseLoc, 1, glm::value_ptr(light.LDiffuse));
	  glUniform3fv(lightSpecularLoc, 1, glm::value_ptr(light.LSpecular));
  }
};

#endif

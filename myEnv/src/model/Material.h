#ifndef _MATERIAL_H_
#define _MATERIAL_H_

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

/* This struct controls materials and its activation
 */
class Material {
  friend class D3;
  glm::vec3 MAmbient;
  glm::vec3 MDiffuse;
  glm::vec3 MSpecular;
  float MShininess;
  // GLint current_shader = -1;
public:
  Material() : MAmbient(glm::vec3(0.0f)),
               MDiffuse(glm::vec3(0.0f)),
               MSpecular(glm::vec3(0.0f)),
               MShininess(0.0f)
              {}
  void Set_Ambient(glm::vec3 new_ambient) {
	  MAmbient = new_ambient;
  }
  Material(glm::vec3 ambient, glm::vec3 diffuse,
           glm::vec3 specular, float shininess) {
    MAmbient = ambient;
    MDiffuse = diffuse;
    MSpecular = specular;
    MShininess = shininess;
  }
  Material(const Material &m) {
    MAmbient = m.MAmbient;
    MDiffuse = m.MDiffuse;
    MSpecular = m.MSpecular;
    MShininess = m.MShininess;
  }
  void Assign(glm::vec3 ambient, glm::vec3 diffuse,
              glm::vec3 specular, float shininess) {
    MAmbient = ambient;
    MDiffuse = diffuse;
    MSpecular = specular;
    MShininess = shininess;
  }
  void MActivate(GLint shader) {
    // Load only once
    // if (current_shader == -1 || shader != current_shader) {
    GLint MAmbientLoc   = glGetUniformLocation(shader, "MAmbient");
    GLint MDiffuseLoc   = glGetUniformLocation(shader, "MDiffuse");
    GLint MSpecularLoc  = glGetUniformLocation(shader, "MSpecular");
    GLint MShininessLoc = glGetUniformLocation(shader, "MShininess");

    glUniform3f(MAmbientLoc,  MAmbient.x,  MAmbient.y,  MAmbient.z);
    glUniform3f(MDiffuseLoc,  MDiffuse.x,  MDiffuse.y,  MDiffuse.z);
    glUniform3f(MSpecularLoc, MSpecular.x, MSpecular.y, MSpecular.z);
    glUniform1f(MShininessLoc, MShininess);

      // current_shader = shader;
    // }

    // getMaterial();
  }
  void getMaterial() {
    std::cout << MAmbient.x << " " << MAmbient.y << " " << MAmbient.z << "\n";
    std::cout << MDiffuse.x << " " << MDiffuse.y << " " << MDiffuse.z << "\n";
    std::cout << MSpecular.x << " " << MSpecular.y << " " << MSpecular.z << "\n";
    std::cout << MShininess << std::endl;
  }
};

#endif

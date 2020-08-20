#ifndef _CAMERA_H_
#define _CAMERA_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#elif __linux__
#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <GL/glut.h>
#else
#include <GL/glew.h>
#endif

#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include "config.h"

extern double lastX, lastY;

class Camera {
	glm::vec3 eye;
	glm::vec3 front;
	glm::vec3 up;
	GLfloat fovy;
	GLfloat aspect;
	GLfloat zNear;
	GLfloat zFar;
	GLfloat Yaw;
	GLfloat Pitch;
	glm::vec3 eye_under_water;
	glm::vec3 front_under_water;
	glm::vec3 up_under_water;
	GLfloat waterHeight = -1.0f;
	GLfloat moveOffset = 0.006f;
	bool reflected = false;
	void calculate_front() {
		glm::vec3 cameraFront;
		cameraFront.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		cameraFront.y = sin(glm::radians(Pitch));
		cameraFront.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front = glm::normalize(cameraFront);
	}
	void calculate_waterReflect() {
		glm::vec3 cameraFront;
		cameraFront.x = cos(glm::radians(Yaw)) * cos(glm::radians(-1*Pitch));
		cameraFront.y = sin(glm::radians(-1*Pitch));
		cameraFront.z = sin(glm::radians(Yaw)) * cos(glm::radians(-1*Pitch));
		front_under_water = glm::normalize(cameraFront);
	}
public:
	std::vector<glm::vec4> planes;

	Camera(glm::vec3 arg_eye, glm::vec3 arg_up,
			GLfloat arg_fov, GLfloat arg_aspect, GLfloat arg_zNear, 
			GLfloat arg_zFar, GLfloat waterH=-1.0f, GLfloat yaw=-90.0f, GLfloat pitch=0.0f) {
		eye = arg_eye;
		up = arg_up;
		fovy = arg_fov;
		aspect = arg_aspect;
		zNear = arg_zNear;
		zFar = arg_zFar;
		this->Yaw = yaw;
		this->Pitch = pitch;
		calculate_front();
		
		/* Update reflection parameters */
		waterHeight = waterH;
		eye_under_water = glm::vec3(eye.x, 2*waterHeight-eye.y, eye.z);
		calculate_waterReflect();

		planes.resize(6);
	}

	glm::vec3 get_eye()		{ return eye; }
	glm::vec3 get_center()	{ return eye+front; }
	glm::vec3 get_up()		{ return up; }
	glm::mat4 get_projection() {
		return glm::perspective(glm::radians(fovy), aspect, zNear, zFar);
	}
	glm::mat4 get_view() {
		if (!reflected)
			return glm::lookAt(eye,eye+front,up);
		return glm::lookAt(eye_under_water, eye_under_water + front_under_water, up);
	}
	void set_aspect(GLfloat arg_aspect) {
		aspect = arg_aspect;
	}
	void set_moveOffset(GLfloat param) {
		moveOffset = param;
	}
	void set_fovy(GLfloat offset) {	
		fovy -= offset;

		if (fovy <= 1.0f)	fovy = 1.0f;
		if (fovy >= 90.0f)	fovy = 90.0f;
	}
	void rotate(GLfloat xoffset, GLfloat yoffset) {
		Yaw += xoffset * 0.35f;
		Pitch += yoffset * 0.35f;

		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;

		calculate_front();
		calculate_waterReflect();
	}
	glm::vec3 moveForward() {
		eye.x += front.x * moveOffset;
		eye.z += front.z * moveOffset;
		eye_under_water.x += front_under_water.x * moveOffset;
		eye_under_water.z += front_under_water.z * moveOffset;

		// std::cout << eye.x << " " << eye.y << " " << eye.z << std::endl;
		return glm::vec3(front.x * moveOffset, 0.0f, front.z * moveOffset);
 	}
	glm::vec3 moveBackward() {
		eye.x -= front.x * moveOffset;
		eye.z -= front.z * moveOffset;
		eye_under_water.x -= front_under_water.x * moveOffset;
		eye_under_water.z -= front_under_water.z * moveOffset;

		return glm::vec3(front.x * moveOffset, 0.0f, front.z * moveOffset);
	}
	bool moveDown() {
		eye.y -= moveOffset*4.0f;
		if (eye.y < 1) {
			eye.y = 1.0f;
			return false;
		}
		eye_under_water.y = 2 * waterHeight - eye.y;
		return true;
	}
	bool moveUp() {
		eye.y += moveOffset * 4.0f;
		eye_under_water.y = 2 * waterHeight - eye.y;
		return true;
	}
	void cameraSpeedUp() { moveOffset *= 2.0f; }
	void cameraSpeedDown() { moveOffset *= 0.5f; }
	void rotate(glm::vec3 rotation) { }
	const std::vector<glm::vec4> &get_frustumPlanes() {
		glm::mat4 matrix = get_projection() * get_view();

		glm::vec4 rowX = glm::row(matrix, 0);
		glm::vec4 rowY = glm::row(matrix, 1);
		glm::vec4 rowZ = glm::row(matrix, 2);
		glm::vec4 rowW = glm::row(matrix, 3);

		planes[0] = normalize(rowW + rowX);
		planes[1] = normalize(rowW - rowX);
		planes[2] = normalize(rowW + rowY);
		planes[3] = normalize(rowW - rowY);
		planes[4] = normalize(rowW + rowZ);
		planes[5] = normalize(rowW - rowZ);

		for (auto &plane : planes) {
			glm::vec3 normal(plane.x, plane.y, plane.z);
			plane = plane / glm::length(normal);
		}

		return planes;
	}
	void reflect() { reflected = !reflected; }
};

#endif

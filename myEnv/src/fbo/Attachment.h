#ifndef _MYATTACHMENT_H_
#define _MYATTACHMENT_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#elif __linux__
#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <GL/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Attachment {
protected:
	GLuint bufferID;
	bool isDepthAttach = false;
public:
	int getBufferId() {
		return bufferID;
	}
	void setBufferId(GLuint id) {
		this->bufferID = id;
	}
	void setAsDepthAttachment() {
		this->isDepthAttach = true;
	}
	bool isDepthAttachment() {
		return isDepthAttach;
	}
	virtual void init(int attachment, int width, int height, int samples) {};
	virtual void del() {};
};

#endif
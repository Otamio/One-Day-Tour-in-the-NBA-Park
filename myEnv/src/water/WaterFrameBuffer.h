#ifndef _WATERFRAMEBUFFER_H_
#define _WATERFRAMEBUFFER_H_

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

class WaterFrameBuffer {
	const int REFLECTION_WIDTH = 320;
	const int REFLECTION_HEIGHT = 180;
	const int REFRACTION_WIDTH = 1280;
	const int REFRACTION_HEIGHT = 720;
	int WINDOW_WIDTH, WINDOW_HEIGHT;
	GLuint reflectionFrameBuffer, reflectionTexture, reflectionDepthBuffer;
	GLuint refractionFrameBuffer, refractionTexture, refractionDepthTexture;
	/* Subroutines - Buffer Creators */
	void initializeReflectionFrameBuffer();
	void initializeRefractionFrameBuffer();
	GLuint createFrameBuffer();
	GLuint createTextureAttachment(int, int);
	GLuint createDepthBufferAttachment(int, int);
	GLuint createDepthTextureAttachment(int, int);
	void bindFrameBuffer(GLuint, int, int);
public:
	WaterFrameBuffer();
	~WaterFrameBuffer();
	void bindReflectionFrameBuffer();
	void bindRefractionFrameBuffer();
	void unbindCurrentFrameBuffer();
	GLuint getReflectionTexture();
	GLuint getRefractionTexture();
	GLuint getRefractionDepthTexture();
};

#endif

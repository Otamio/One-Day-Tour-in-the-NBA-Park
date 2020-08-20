#ifndef _MYFBO_H_
#define _MYFBO_H_

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
#include <map>
#include "Attachment.h"
#include "RenderBufferAttachment.h"
#include <iostream>

class Fbo;
class FboMsBuilder;
class FboBuilder;

class Fbo {
	int fboId, width, height;
	std::map<int, Attachment *> colorAttachments;
	Attachment *depthAttachment = nullptr;
public:
	Fbo(int fboId, int width, int height, std::map<int, Attachment *> attachments,
		Attachment *depthAttachment) {
		this->fboId = fboId;
		this->width = width;
		this->height = height;
		this->colorAttachments = attachments;
		this->depthAttachment = depthAttachment;
	}
	~Fbo() {
		del();
	}
	void blitToScreen(int colorIndex) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glDrawBuffer(GL_BACK);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fboId);
		glReadBuffer(GL_COLOR_ATTACHMENT0 + colorIndex);
		glBlitFramebuffer(0, 0, width, height, 0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT),
			GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void blitToFbo(int srcColorIndex, Fbo target, int targetColorIndex) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target.fboId);
		glDrawBuffer(GL_COLOR_ATTACHMENT0 + targetColorIndex);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fboId);
		glReadBuffer(GL_COLOR_ATTACHMENT0 + srcColorIndex);

		int bufferBit = depthAttachment != nullptr && target.depthAttachment != nullptr
			? GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT : GL_COLOR_BUFFER_BIT;
		glBlitFramebuffer(0, 0, width, height, 0, 0, target.width, target.height, bufferBit, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	int getColorBuffer(int colorIndex) {
		return colorAttachments.at(colorIndex)->getBufferId();
	}
	int getDepthBuffer() {
		return depthAttachment->getBufferId();
	}
	bool isComplete() {
		glBindFramebuffer(GL_FRAMEBUFFER, fboId);
		bool complete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return complete;
	}
	void bindForRender(int colorIndex) {
		glDrawBuffer(GL_COLOR_ATTACHMENT0 + colorIndex);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboId);
		glViewport(0, 0, width, height);
	}
	void unbindAfterRender(int window_width, int window_height) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glViewport(0, 0, window_width, window_height);
	}
	void del() {
		for (auto entry = colorAttachments.begin(); entry != colorAttachments.end(); ++entry) {
			entry->second->del();
		}
		if (depthAttachment != nullptr) {
			depthAttachment->del();
		}
	}
};

class FboBuilder {
	int width, height, samples;
	std::map<int, Attachment *> colorAttachments;
	Attachment *depthAttachment = nullptr;
	void createColorAttachments() {
		for (auto entry = colorAttachments.begin(); entry != colorAttachments.end(); ++entry) {
			Attachment *attachment = entry->second;
			int attachmentId = GL_COLOR_ATTACHMENT0 + entry->first;
			attachment->init(attachmentId, width, height, samples);
		}
	}
	void createDepthAttachment() {
		if (depthAttachment != nullptr) {
			depthAttachment->init(GL_DEPTH_ATTACHMENT, width, height, samples);
		}
	}
public:
	FboBuilder(int width, int height) {
		this->width = width;
		this->height = height;
		this->samples = 0;
	}
	FboBuilder(int width, int height, int samples) {
		this->width = width;
		this->height = height;
		this->samples = samples;
	}
	FboBuilder *addColorAttachment(int index, Attachment *attachment) {
		colorAttachments.insert({index, attachment});
		return this;
	}
	FboBuilder *addDepthAttachment(Attachment *attachment) {
		this->depthAttachment = attachment;
		attachment->setAsDepthAttachment();
		return this;
	}
	Fbo *init() {
		int fboId = createFbo();
		createColorAttachments();
		createDepthAttachment();
		return new Fbo(fboId, width, height, colorAttachments, depthAttachment);
	}
	GLuint createFbo() {
		GLuint fboId;
		glGenFramebuffers(1, &fboId);
		glBindFramebuffer(GL_FRAMEBUFFER, fboId);
		return fboId;
	}
};

class FboMsBuilder {
	FboBuilder *fboBuilder;
public:
	FboMsBuilder(int width, int height, int samples) {
		this->fboBuilder = new FboBuilder(width, height, samples);
	}
	FboMsBuilder(FboBuilder *fboBuilder) {
		this->fboBuilder = fboBuilder;
	}
	FboMsBuilder *addColorAttachment(int index, RenderBufferAttachment *attachment) {
		fboBuilder->addColorAttachment(index, attachment);
		return this;
	}
	FboMsBuilder *addDepthAttachment(RenderBufferAttachment *attachment) {
		fboBuilder->addDepthAttachment(attachment);
		return this;
	}
	Fbo *init() {
		return fboBuilder->init();
	}
};
#endif

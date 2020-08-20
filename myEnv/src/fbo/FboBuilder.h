#ifndef _FBO_BUILDER_H_
#define _FBO_BUILDER_H_

#include "Attachment.h"
#include "FBO.h"
#include <map>

class FboBuilder {
	int width, height, samples;
	std::map<int, Attachment *> colorAttachments;
	Attachment *depthAttachment = nullptr;
	void createColourAttachments() {
		for (auto entry=colorAttachments.begin(); entry!=colorAttachments.end(); ++entry) {
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
	FboBuilder() {}
	FboBuilder(int width, int height, int samples) {
		this->width = width;
		this->height = height;
		this->samples = samples;
	}
	FboBuilder addColorAttachment(int index, Attachment *attachment) {
		colorAttachments.emplace(index, *attachment);
		return *this;
	}
	FboBuilder addDepthAttachment(Attachment *attachment) {
		this->depthAttachment = attachment;
		attachment->setAsDepthAttachment();
		return *this;
	}
	Fbo init() {
		int fboId = createFbo();
		createColourAttachments();
		createDepthAttachment();
		return Fbo(fboId, width, height, colorAttachments, depthAttachment);
	}
	GLuint createFbo() {
		GLuint fboId;
		glGenFramebuffers(1, &fboId);
		glBindFramebuffer(GL_FRAMEBUFFER, fboId);
		return fboId;
	}
};

#endif

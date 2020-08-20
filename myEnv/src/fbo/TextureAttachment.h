#ifndef _TEXTURE_ATTACHMENT_H_
#define _TEXTURE_ATTACHMENT_H_

#include "Attachment.h"

class TextureAttachment : public Attachment {
	int format;
	bool nearestFiltering;
	bool clampEdges;
public:
	TextureAttachment(int format) {
		this->format = format;
		this->nearestFiltering = false;
		this->clampEdges = false;
	}

	TextureAttachment(int format, bool nearestFiltering, bool clampEdges) {
		this->format = format;
		this->nearestFiltering = nearestFiltering;
		this->clampEdges = clampEdges;
	}

	void del() override {
		glDeleteTextures(1, &(Attachment::bufferID));
	}

	void init(int attachment, int width, int height, int samples) override {
		GLuint texture;
		glGenTextures(1, &texture);
		Attachment::setBufferId(texture);
		
		glBindTexture(GL_TEXTURE_2D, texture);
		indicateStorageType(width, height);
		setTextureParams();
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, 0);
	}

	void indicateStorageType(int width, int height) {
		if (isDepthAttachment()) {
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		}
	}

	void setTextureParams() {
		int filterType = nearestFiltering ? GL_NEAREST : GL_LINEAR;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterType);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterType);
		int wrapType = clampEdges ? GL_CLAMP_TO_EDGE : GL_REPEAT;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapType);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapType);
	}
};

#endif

#ifndef _RENDER_BUFFER_ATTACHMENT_H_
#define _RENDER_BUFFER_ATTACHMENT_H_

#include "Attachment.h"

class RenderBufferAttachment : public Attachment {
	int format;
public:
	RenderBufferAttachment(int format) {
		this->format = format;
	}
	void init(int attachment, int width, int height, int samples) override {
		GLuint buffer;
		glGenRenderbuffers(1, &buffer);
		Attachment::setBufferId(buffer);

		glBindRenderbuffer(GL_RENDERBUFFER, buffer);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, format, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, buffer);
	}

	void del() override {
		glDeleteRenderbuffers(1, &(Attachment::bufferID));
	}
};

#endif
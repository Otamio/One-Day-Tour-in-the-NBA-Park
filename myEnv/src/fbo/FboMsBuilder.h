#ifndef _FBOMSBUILDER_H_
#define _FBOMSBUILDER_H_

#include "FboBuilder.h"
#include "RenderBufferAttachment.h"

class FboMsBuilder {
	FboBuilder fboBuilder;
public:
	FboMsBuilder(FboBuilder fboBuilder) {
		this->fboBuilder = fboBuilder;
	}
	FboMsBuilder addColorAttachment(int index, RenderBufferAttachment *attachment) {
		fboBuilder.addColorAttachment(index, attachment);
		return *this;
	}
	FboMsBuilder addDepthAttachment(RenderBufferAttachment *attachment) {
		fboBuilder.addDepthAttachment(attachment);
		return *this;
	}
	Fbo init() {
		return fboBuilder.init();
	}
};

#endif
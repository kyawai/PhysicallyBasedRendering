#include "Core.h"
#include <stb_image.h>
#include <iostream>

void Core::SetFrameBuffers()
{
		glGenFramebuffers(1, &captureFBO);
		glGenRenderbuffers(1, &captureRBO);
		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1024, 1024);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
}


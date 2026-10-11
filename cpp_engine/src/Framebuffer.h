#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glad/glad.h>
#include <iostream>

class Framebuffer {
public:
    unsigned int fboId = 0;
    unsigned int textureId = 0;
    unsigned int rboId = 0;
    int width = 0;
    int height = 0;

    Framebuffer(int w, int h) : width(w), height(h) {
        // 1. Generate and bind our Framebuffer container
        glGenFramebuffers(1, &fboId);
        glBindFramebuffer(GL_FRAMEBUFFER, fboId);

        // 2. Generate and configure the Color Attachment Texture
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Attach color texture to the frame buffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);

        // 3. Generate and configure the Renderbuffer for Depth & Stencil checks
        glGenRenderbuffers(1, &rboId);
        glBindRenderbuffer(GL_RENDERBUFFER, rboId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // Attach renderbuffer to depth and stencil attachment channels
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboId);

        // 4. Validate Framebuffer Completeness
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        }

        // Return render target context to default backbuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    ~Framebuffer() {
        glDeleteFramebuffers(1, &fboId);
        glDeleteTextures(1, &textureId);
        glDeleteRenderbuffers(1, &rboId);
    }

    // Call before drawing your 3D world to direct draw calls into this offscreen texture
    void Bind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, fboId);
        glViewport(0, 0, width, height);
    }

    // Call after drawing your 3D world to restore default screen rendering
    void Unbind(int screenWidth, int screenHeight) const {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, screenWidth, screenHeight);
    }
};

#endif // FRAMEBUFFER_H
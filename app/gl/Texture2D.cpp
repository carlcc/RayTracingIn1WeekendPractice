/**
 * Copyright (c) 2018
 * Carl Chen. All rights reserved.
 *
 * This file is part of MSServer.
 * Created by Chen Chen on 2018/12/24.
 */

#include "Texture2D.h"
#include "Log.h"

Texture2D::Texture2D(): mTexture(0)
{
}

Texture2D::~Texture2D()
{
    if (mTexture != 0) {
        glDeleteTextures(1, &mTexture);
    }
}

bool Texture2D::initialize()
{
    if (mTexture != 0) {
        glDeleteTextures(1, &mTexture);
    }

    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT	);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT	);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;
}

void Texture2D::bufferImage(const void* image, int width, int height, Texture2D::Format format) const
{
    glBindTexture(GL_TEXTURE_2D, mTexture);
    GLenum fmt;
    switch (format) {
        case Format::RGB888:
            fmt = GL_RGB;
            break;
        case Format::RGBA8888:
            fmt = GL_RGBA;
            break;
        default:
            ASSERT(false, "Not supported yet!");
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, fmt, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture2D::active(GLenum slot)
{
    glActiveTexture(slot);
}

Texture2D::operator bool() const
{
    return mTexture != 0;
}

void Texture2D::bind() const
{
    glBindTexture(GL_TEXTURE_2D, mTexture);
}


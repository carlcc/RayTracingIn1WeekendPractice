/**
 * Copyright (c) 2018
 * Carl Chen. All rights reserved.
 *
 * This file is part of MSServer.
 * Created by Chen Chen on 2018/12/24.
 */

#pragma once


#include <glad/glad.h>

class Texture2D {
public:
    enum Format {
        RGB888,
        RGBA8888
    };

    Texture2D();
    ~Texture2D();

    bool initialize();

    void bind() const;

    void bufferImage(const void* image, int width, int height, Format format) const;

    GLuint getId() const { return mTexture; }

    operator bool() const;

    static void active(GLenum slot);

private:
    GLuint mTexture;
};




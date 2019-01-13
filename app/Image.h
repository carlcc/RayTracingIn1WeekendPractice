/**
 * Copyright (c) 2018
 * Carl Chen. All rights reserved.
 *
 * This file is part of MSServer.
 * Created by Chen Chen on 2018/12/16.
 */

#pragma once

#include <string>

class Image {
public:
    Image();
    Image(int width, int height, void* data = nullptr, int channelCount = 3);
    ~Image();

    void reallocate(int width, int height, void* data = nullptr, int channelCount = 3);

    int getChannelCount() const { return mChannelCount; }

    int getWidth() const { return mWidth; }

    int getHeight() const { return mHeight; }

    char* getData() const { return mBuffer; }

    bool load(const std::string& path, bool flipVertically = true);

    bool write(const std::string& path, const std::string& format);

private:

    char* mBuffer;
    int mWidth;
    int mHeight;
    int mChannelCount;

};




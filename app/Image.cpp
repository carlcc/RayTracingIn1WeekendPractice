/**
 * Copyright (c) 2018
 * Carl Chen. All rights reserved.
 *
 * This file is part of MSServer.
 * Created by Chen Chen on 2018/12/16.
 */

#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <cmath>
#include <algorithm>
#include "stb_image.h"
#include "stb_image_write.h"

Image::Image() : mBuffer(nullptr), mWidth(-1), mHeight(-1), mChannelCount(-1)
{
}

Image::Image(int width, int height, void *data, int channelCount):
        mWidth(width), mHeight(height), mBuffer(new char[width*height*channelCount]), mChannelCount(channelCount)
{
    if (data != nullptr) {
        memcpy(mBuffer, data, (size_t) (width * height * channelCount));
    }
}

Image::~Image()
{
    delete[] mBuffer;
}

bool Image::load(const std::string& path, bool flipVertically)
{
    delete[] mBuffer;

    stbi_set_flip_vertically_on_load(flipVertically ? 1 : 0);
    mBuffer = reinterpret_cast<char*>(stbi_load(path.c_str(), &mWidth, &mHeight, &mChannelCount, 0));
    return mBuffer != nullptr;
}

bool Image::write(const std::string &path, const std::string& format)
{
    std::string fmt = format;
    std::transform(fmt.begin(), fmt.end(), fmt.begin(), ::tolower);

    if (format == "png") {
        return 1 == stbi_write_png(path.c_str(), mWidth, mHeight, mChannelCount, mBuffer, mChannelCount * mWidth);
    }
    if (format == "jpg") {
        return 1 == stbi_write_jpg(path.c_str(), mWidth, mHeight, mChannelCount, mBuffer, 100);
    }
    if (format == "bmp") {
        return 1 == stbi_write_bmp(path.c_str(), mWidth, mHeight, mChannelCount, mBuffer);
    }
    if (format == "tga") {
        return 1 == stbi_write_tga(path.c_str(), mWidth, mHeight, mChannelCount, mBuffer);
    }

    return false;
}

void Image::reallocate(int width, int height, void *data, int channelCount)
{
    if (mBuffer != nullptr) {
        delete[] mBuffer;
    }

    mWidth = width;
    mHeight = height;
    mBuffer = new char[width * height * channelCount];
    mChannelCount = channelCount;

    if (data != nullptr) {
        memcpy(mBuffer, data, (size_t) (width * height * channelCount));
    }
}

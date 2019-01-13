//
// Created by carl on 19-1-13.
//

#include <iostream>
#include "Image.h"
#include "Paths.h"

int main()
{
    Image image;
    if (!image.load(CURRENT_DIRECTORY + "face.png", true)) {
        std::cout << "Load failed'\n";
    }

    if (!image.write(CURRENT_DIRECTORY + "faceout.png", "png")) {
        std::cout << "Write failed" << std::endl;
    }

    if (!image.write(CURRENT_DIRECTORY + "faceout.jpg", "jpg")) {
        std::cout << "Write failed" << std::endl;
    }

    if (!image.write(CURRENT_DIRECTORY + "faceout.bmp", "bmp")) {
        std::cout << "Write failed" << std::endl;
    }

    if (!image.write(CURRENT_DIRECTORY + "faceout.tga", "tga")) {
        std::cout << "Write failed" << std::endl;
    }

    return 0;
}
/**
 * Copyright (c) 2018
 * Carl Chen. All rights reserved.
 *
 * This file is part of MSServer.
 * Created by carl on ${DATA}.
 */

#pragma once

#include <string>

inline std::string getFileDirectory(const char* f)
{
    int index = -1;
    for (int i = 0; f[i] != 0; ++i) {
        if (f[i] == '/' || f[i] == '\\') {
            index = i;
        }
    }
    return std::string(f, index + 1);
}

#define CURRENT_DIRECTORY (getFileDirectory(__FILE__))
#define OFFSET(off) ((void*)(off))
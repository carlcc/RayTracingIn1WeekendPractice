/**
 * Copyright (c) 2018
 * Carl Chen. All rights reserved.
 *
 * This file is part of MSServer.
 * Created by Chen Chen on 2018/12/16.
 */

#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

struct ShaderInfo {
    GLenum       type;
    std::string  filename;
    GLuint       shader;
};

struct ShaderInfo2 {
    GLenum        type;
    std::string   source;
    GLuint        shader;
};

class ShaderProgram {
public:
    ShaderProgram();
    ~ShaderProgram();

    bool load(ShaderInfo* shaders);

    bool load(ShaderInfo2* shaders);

    void use() const;

    void setInt(const std::string& name, int value) const;

    void setFloat(const std::string& name, float value) const;

    void setVec3(const std::string& name, const float* values) const;

    void setVec3(const std::string& name, float v1, float v2, float v3) const;

    void setVec3(const std::string& name, glm::vec3 values) const;

    void setVec4(const std::string& name, const float* values) const;

    void setVec4(const std::string& name, float v1, float v2, float v3, float v4) const;

    void setVec4(const std::string& name, const glm::vec4 values) const;

    void setMat4(const std::string& name, const float* values) const;

    void setMat4(const std::string& name, const glm::mat4& values) const;

public:
    GLuint mProgram;
};




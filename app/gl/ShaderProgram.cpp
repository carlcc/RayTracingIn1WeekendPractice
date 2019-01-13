/**
 * Copyright (c) 2018
 * Carl Chen. All rights reserved.
 *
 * This file is part of MSServer.
 * Created by Chen Chen on 2018/12/16.
 */

#include "ShaderProgram.h"
#include "Log.h"

#include <iostream>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

ShaderProgram::ShaderProgram() : mProgram(0)
{

}

ShaderProgram::~ShaderProgram()
{
    if (mProgram != 0) {
        glDeleteProgram(mProgram);
        mProgram = 0;
    }
}

static const GLchar*
ReadShader(const char* filename)
{
#ifdef WIN32
    FILE* infile;
    fopen_s( &infile, filename, "rb" );
#else
    FILE* infile = fopen(filename, "rb");
#endif // WIN32

    if (!infile) {
        FATAL("Unable to open file '{}'", filename);

        return nullptr;
    }

    fseek(infile, 0, SEEK_END);
    int len = ftell(infile);
    fseek(infile, 0, SEEK_SET);

    char* source = new GLchar[len + 1];

    fread(source, 1, len, infile);
    fclose(infile);

    source[len] = 0;

    return const_cast<const GLchar*>(source);
}

bool ShaderProgram::load(ShaderInfo* shaders)
{
    if (shaders == nullptr) {
        return false;
    }

    GLuint program = glCreateProgram();

    ShaderInfo* entry = shaders;
    while (entry->type != GL_NONE) {
        GLuint shader = glCreateShader(entry->type);

        entry->shader = shader;

        const char* source = ReadShader(entry->filename.c_str());
        if (source == nullptr) {
            for (entry = shaders; entry->type != GL_NONE; ++entry) {
                glDeleteShader(entry->shader);
                entry->shader = 0;
            }

            FATAL("Read Shader source failed");
            return false;
        }

        glShaderSource(shader, 1, &source, nullptr);
        delete[] source;

        glCompileShader(shader);

        GLint compiled;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLsizei len;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

            auto* log = new GLchar[len + 1];
            glGetShaderInfoLog(shader, len, &len, log);
            FATAL("Shader compilation failed: {}, {}", entry->filename, log);
            delete[] log;

            return false;
        }

        glAttachShader(program, shader);

        ++entry;
    }

    glLinkProgram(program);

    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLsizei len;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

        GLchar* log = new GLchar[len + 1];
        glGetProgramInfoLog(program, len, &len, log);
        FATAL("Shader linking failed: ", log);
        delete[] log;

        for (entry = shaders; entry->type != GL_NONE; ++entry) {
            glDeleteShader(entry->shader);
            entry->shader = 0;
        }

        return false;
    }

    mProgram = program;
    return true;
}

bool ShaderProgram::load(ShaderInfo2* shaders)
{
    if (shaders == nullptr) {
        return false;
    }

    GLuint program = glCreateProgram();

    ShaderInfo2* entry = shaders;
    while (entry->type != GL_NONE) {
        GLuint shader = glCreateShader(entry->type);

        entry->shader = shader;

        const char* source = entry->source.c_str();

        glShaderSource(shader, 1, &source, nullptr);

        glCompileShader(shader);

        GLint compiled;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLsizei len;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

            auto* log = new GLchar[len + 1];
            glGetShaderInfoLog(shader, len, &len, log);
            FATAL("Shader compilation failed: {}, {}", entry->source, log);
            delete[] log;

            return false;
        }

        glAttachShader(program, shader);

        ++entry;
    }

    glLinkProgram(program);

    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLsizei len;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

        GLchar* log = new GLchar[len + 1];
        glGetProgramInfoLog(program, len, &len, log);
        FATAL("Shader linking failed: ", log);
        delete[] log;

        for (entry = shaders; entry->type != GL_NONE; ++entry) {
            glDeleteShader(entry->shader);
            entry->shader = 0;
        }

        return false;
    }

    mProgram = program;
    return true;
}

void ShaderProgram::use() const
{
    glUseProgram(mProgram);
}

void ShaderProgram::setInt(const std::string& name, int value) const
{
    auto location = glGetUniformLocation(mProgram, name.c_str());
    glUniform1i(location, value);
}

void ShaderProgram::setFloat(const std::string& name, float value) const
{
    auto location = glGetUniformLocation(mProgram, name.c_str());
    glUniform1f(location, value);
}

void ShaderProgram::setVec3(const std::string& name, const float* values) const
{
    auto location = glGetUniformLocation(mProgram, name.c_str());
    glUniform3fv(location, 1, values);
}

void ShaderProgram::setVec3(const std::string& name, float v1, float v2, float v3) const
{
    float tmp[] = {v1, v2, v3};
    auto location = glGetUniformLocation(mProgram, name.c_str());
    glUniform3fv(location, 1, tmp);
}

void ShaderProgram::setVec3(const std::string& name, glm::vec3 values) const
{
    auto location = glGetUniformLocation(mProgram, name.c_str());
    glUniform3fv(location, 1, &values[0]);
}

void ShaderProgram::setVec4(const std::string& name, const float* values) const
{
    auto location = glGetUniformLocation(mProgram, name.c_str());
    glUniform4fv(location, 1, values);
}

void ShaderProgram::setVec4(const std::string& name, float v1, float v2, float v3, float v4) const
{
    float tmp[] = {v1, v2, v3, v4};
    auto location = glGetUniformLocation(mProgram, name.c_str());
    glUniform4fv(location, 1, tmp);
}

void ShaderProgram::setVec4(const std::string& name, const glm::vec4 values) const
{
    auto location = glGetUniformLocation(mProgram, name.c_str());
    glUniform4fv(location, 1, &values[0]);
}

void ShaderProgram::setMat4(const std::string& name, const float* value) const
{
    auto location = glGetUniformLocation(mProgram, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, value);
}

void ShaderProgram::setMat4(const std::string& name, const glm::mat4& values) const
{
    setMat4(name, glm::value_ptr(values));
}

#pragma once
#include <glad/glad.h>
#ifndef SHADER_H
#define SHADER_H



class Shader {
public:
    static char *readShaderFile(const char *path);
    static GLuint compileShader(GLenum type, const char *source);
};


#endif //SHADER_H

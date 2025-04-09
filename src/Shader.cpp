#include "Shader.h"
#include <fstream>
#include <string>

char * Shader::readShaderFile(const char *path) {
    std::ifstream file(path);
    std::string line;
    std::string shaderSource;
    while(std::getline(file, line)) {
        shaderSource += line + "\n";
    }
    char* shaderSourceChar = new char[shaderSource.length() + 1];
    strcpy(shaderSourceChar, shaderSource.c_str());
    return shaderSourceChar;
}

GLuint Shader::compileShader(GLenum type, const char* source) {
    const GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        fprintf(stderr, "Shader Compilation Failed: %s\n", infoLog);
    }
    return shader;
}
#include "openGL/program.h"
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <vector>
#include "file_utils.h"

GLshader::GLshader(GLuint shaderType) : address(glCreateShader(shaderType)), shaderType(shaderType) {}

GLshader::GLshader(GLshader &&shader) : address(shader.address), shaderType(shader.shaderType) {
    shader.address = 0;
}

GLshader::~GLshader() {
    glDeleteShader(address);
}

void GLshader::setCode(const std::string &code) {
    const char *codeCStr = code.c_str();
    glShaderSource(address, 1, &codeCStr, nullptr);
    glCompileShader(address);

    GLint isCompiled = 0;
    glGetShaderiv(address, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled) {
        GLint maxLength = 0;
        glGetShaderiv(address, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<char> errorLog(maxLength);
        glGetShaderInfoLog(address, maxLength, &maxLength, &errorLog[0]);
        std::cerr << "Shader compilation failed: " << &errorLog[0] << std::endl;
    }
}

void GLshader::setCodeFromFile(const std::filesystem::path &path) {
    std::string code = file_utils::read(path);
    setCode(code);
}

GLprogram::GLprogram() : address(glCreateProgram()) {}

GLprogram::GLprogram(GLprogram &&program) : address(program.address) {
    program.address = 0;
}

GLprogram::~GLprogram() {
    glDeleteProgram(address);
}

GLprogram& GLprogram::addShader(const GLshader &shader) {
    glAttachShader(address, shader.getID());
    return *this;
}

void GLprogram::link() {
    glLinkProgram(address);

    GLint isLinked = 0;
    glGetProgramiv(address, GL_LINK_STATUS, &isLinked);
    if (!isLinked) {
        GLint maxLength = 0;
        glGetProgramiv(address, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<char> infoLog(maxLength);
        glGetProgramInfoLog(address, maxLength, &maxLength, &infoLog[0]);
        std::cerr << "Program linking failed: " << &infoLog[0] << std::endl;
    }
}

void GLprogram::use() {
    glUseProgram(address);
}

GLint GLprogram::getUniformLocation(const std::string &name) {
    return glGetUniformLocation(address, name.c_str());
}

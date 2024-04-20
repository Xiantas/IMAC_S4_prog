#include "openGL/program.h"

#include <glad/glad.h>

#include <iostream>
#include <string>
#include <vector>

#include "file_utils.h"

GLshader::GLshader(GLuint shaderType)
    : address(glCreateShader(shaderType)), shaderType(shaderType) {}

GLshader::GLshader(GLshader&& shader)
    : address(shader.address), shaderType(shader.shaderType) {
    shader.address = 0;
}

GLshader::~GLshader() {
    glDeleteShader(address);
}

void GLshader::setCode(const std::string& code) {
    const char* codeCStr = code.c_str();
    glShaderSource(this->address, 1, &codeCStr, nullptr);
    glCompileShader(this->address);

    GLint isCompiled = 0;
    glGetShaderiv(this->address, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled) {
        GLint maxLength = 0;
        glGetShaderiv(this->address, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<char> errorLog(maxLength);
        glGetShaderInfoLog(this->address, maxLength, &maxLength, &errorLog[0]);
        std::cerr << "Shader compilation failed: " << &errorLog[0] << std::endl;
    }
}

void GLshader::setCodeFromFile(const std::filesystem::path& path) {
    std::string code = file_utils::read(path);
    setCode(code);
}

GLprogram::GLprogram()
    : address(glCreateProgram()) {}

GLprogram::GLprogram(GLprogram&& program)
    : address(program.address) {
    program.address = 0;
}

GLprogram::~GLprogram() {
    glDeleteProgram(this->address);
}

GLprogram& GLprogram::addShader(const GLshader& shader) {
    glAttachShader(this->address, shader.getID());
    return *this;
}

void GLprogram::link() {
    glLinkProgram(this->address);

    GLint isLinked = 0;
    glGetProgramiv(this->address, GL_LINK_STATUS, &isLinked);
    if (!isLinked) {
        GLint maxLength = 0;
        glGetProgramiv(this->address, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<char> infoLog(maxLength);
        glGetProgramInfoLog(this->address, maxLength, &maxLength, &infoLog[0]);
        std::cerr << "Program linking failed: " << &infoLog[0] << std::endl;
    }
}

void GLprogram::use() {
    glUseProgram(this->address);
}

GLint GLprogram::getUniformLocation(const std::string& name) {
    return glGetUniformLocation(this->address, name.c_str());
}

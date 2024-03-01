#include "openGL/program.h"

#include <glad/glad.h>

#include <string>
#include "file_utils.h"

#include <iostream>

GLshader::GLshader(GLuint shaderType)
    : address(glCreateShader(shaderType))
    , shaderType(shaderType)
{}

GLshader::GLshader(GLshader &&shader)
    : address(shader.address)
{
    shader.address = 0;
}

GLshader::~GLshader() {
    glDeleteShader(this->address);
}

void GLshader::setCode(std::string const &code) {
    auto chars = code.c_str();
    glShaderSource(this->address, 1, &chars, nullptr);
    glCompileShader(this->address);

    GLint LATAILELE;
    glGetShaderiv(this->address, GL_INFO_LOG_LENGTH, &LATAILELE);
    std::string AGAGA;
    AGAGA.reserve(LATAILELE);
    glGetShaderInfoLog(
        this->address,
        LATAILELE,
        nullptr,
        AGAGA.data()
    );

    std::cout << AGAGA << '\n';
}
            
void GLshader::setCodeFromFile(std::filesystem::path const &path) {
    auto code = file_utils::read(path);

    this->setCode(code);
}


GLprogram::GLprogram()
    : address(glCreateProgram())
{}

GLprogram::GLprogram(GLprogram &&program)
    : address(program.address)
{
    program.address = 0;
}

GLprogram::~GLprogram() {
    glDeleteProgram(this->address);
}


auto GLprogram::addShader(GLshader const &shader) -> GLprogram& {
    glAttachShader(this->address, shader.address);

    return *this;
}

void GLprogram::link() {
    glLinkProgram(this->address);

    GLint LATAILELE;
    glGetProgramiv(this->address, GL_INFO_LOG_LENGTH, &LATAILELE);
    std::string AGAGA;
    AGAGA.reserve(LATAILELE);
    glGetProgramInfoLog(
        this->address,
        LATAILELE,
        nullptr,
        AGAGA.data()
    );

    std::cout << AGAGA << '\n';
}

void GLprogram::use() {
    glUseProgram(this->address);
}

#include "openGL/program.h"

#include <glad/glad.h>

GLshader::GLshader(GLuint shaderType)
    : shaderType(glCreateShader(shaderType)) 
{}

GLshader::GLshader(GLshader &&shader)
    : address(shader.address)
{
    shader.address = 0;
}

GLshader::~GLshader() {
    glDeleteShader(this->address);
}

#pragma once

#include <glad/glad.h>

class GLshader {
public :
    GLshader(GLuint shaderType);
    GLshader(GLshader const &shader) = delete;
    GLshader(GLshader &&shader);

    ~GLshader();

private :
    GLuint address;
    Gluint shaderType;
};

class GLprogram {
public :
private :
    GLuint address;
};

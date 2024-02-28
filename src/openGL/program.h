#pragma once

#include <glad/glad.h>

#include <string>
#include <filesystem>

class GLshader {
public :
    GLshader(GLuint shaderType);
    GLshader(GLshader const &shader) = delete;
    GLshader(GLshader &&shader);

    void setCode(std::string const &code);

    void stdCodeFromFile(std::filesystem::path const &path);

    ~GLshader();

private :
    GLuint address;
    GLuint shaderType;
};

class GLprogram {
public :
private :
    GLuint address;
};

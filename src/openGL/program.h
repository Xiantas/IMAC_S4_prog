#pragma once

#include <glad/glad.h>

#include <string>
#include <filesystem>

class GLprogram;

class GLshader {
public :
    GLshader(GLuint shaderType);
    GLshader(GLshader const &shader) = delete;
    GLshader(GLshader &&shader);

    ~GLshader();

    //TODO error handling
    void setCode(std::string const &code);

    void setCodeFromFile(std::filesystem::path const &path);

    friend class GLprogram;

private :
    GLuint address;
    GLuint shaderType;
};

class GLprogram {
public :
    GLprogram();
    GLprogram(GLprogram const &program) = delete;
    GLprogram(GLprogram &&program);

    ~GLprogram();

    auto addShader(GLshader const &shader) -> GLprogram&;

    void link();
    void use();
private :
    GLuint address;
};

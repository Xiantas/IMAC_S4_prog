#pragma once

#include <glad/glad.h>
#include <string>
#include <filesystem>

class GLprogram;

class GLshader {
public:
    GLshader(GLuint shaderType);
    GLshader(GLshader const &shader) = delete;
    GLshader(GLshader &&shader);
    ~GLshader();

    void setCode(const std::string &code);
    void setCodeFromFile(const std::filesystem::path &path);
    GLuint getID() const { return address; }

private:
    GLuint address;
    GLuint shaderType;
};

class GLprogram {
public:
    GLprogram();
    GLprogram(const GLprogram &program) = delete;
    GLprogram(GLprogram &&program);
    ~GLprogram();

    GLprogram& addShader(const GLshader &shader);
    void link();
    void use();
    GLuint getID() const { return address; }
    GLint getUniformLocation(const std::string &name);

private:
    GLuint address;
};

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
    auto getID() const -> GLuint { return this->address; }

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

    auto addShader(const GLshader &shader) -> GLprogram&;
    void link();
    void use() const;
    auto getID() const -> GLuint { return this->address; }
    auto getUniformLocation(const std::string &name) const -> GLint;

private:
    GLuint address;
};

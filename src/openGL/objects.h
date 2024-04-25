#pragma once

#include <glad/glad.h>
#include <span>
#include <filesystem>
#include <optional>
#include "vertexData.h"

class GLvao;

enum class VaoType { Classic, Instanced };

class GLvbo {
public:
    GLvbo();
    GLvbo(GLvbo const &vbo) = delete;
    GLvbo(GLvbo &&vbo);
    ~GLvbo();

    void setData(std::span<VertexData> slice);
    void bind();

    auto getVertexCount() const -> GLuint { return this->vertexCount; }

private:
    friend class GLvao;
    GLvbo(bool);

    GLuint address;
    GLsizei vertexCount;
};

class GLvao {
public:
    GLvao() = delete;
    GLvao(VaoType vaoType);
    GLvao(GLvao const &vao) = delete;
    GLvao(GLvao &&vao);
    ~GLvao();

    void setVboMain(std::span<VertexData> slice);
    void setVboInstance(std::span<glm::vec3> slice);
    auto getType() const -> VaoType;
    void draw() const;

private:
    GLuint address;


    VaoType vaoType;
    GLvbo vboMain;
    GLvbo vboInstance;
};

class GLtexture {
public:
    GLtexture();
    GLtexture(GLtexture const &texture) = delete;
    GLtexture(GLtexture &&texture);

    void setData(GLint width, GLint height, int nbChannels, unsigned char *data);
    void bind() const;

private:
    GLuint address;
};

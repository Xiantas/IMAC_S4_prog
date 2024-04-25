#include "objects.h"

#include <glad/glad.h>
#include <span>
#include <optional>
#include "vertexData.h"

GLvbo::GLvbo() : vertexCount(0) {
    glGenBuffers(1, &this->address);
}

GLvbo::GLvbo(GLvbo &&vbo)
    : address(vbo.address)
{
    vbo.address = 0;
}

GLvbo::~GLvbo() {
    glDeleteBuffers(1, &this->address);
}

void GLvbo::setData(std::span<VertexData> slice) {
    glBindBuffer(GL_ARRAY_BUFFER, this->address);
    glBufferData(GL_ARRAY_BUFFER, slice.size()*sizeof(VertexData), slice.data(), GL_STATIC_DRAW);
    this->vertexCount = slice.size();
}

void GLvbo::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, this->address);
}


GLvao::GLvao(VaoType vaoType)
    : vaoType(vaoType)
{
    glGenVertexArrays(1, &this->address);
    glBindVertexArray(this->address);

    this->vboMain.bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)(6*sizeof(GLfloat)));

    if (vaoType == VaoType::Instanced) {
        this->vboInstance.bind();
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(
            3, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)0);
        glVertexAttribDivisor(3,1);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(
            4, 3, GL_FLOAT, GL_FALSE,
            6*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
        glVertexAttribDivisor(4,1);
    }
}

GLvao::GLvao(GLvao &&vao)
    : address(vao.address)
{
    vao.address = 0;
}

GLvao::~GLvao() {
    glDeleteVertexArrays(1, &this->address);
}

void GLvao::setVboMain(std::span<VertexData> slice) {
    this->vboMain.setData(slice);
}

GLtexture::GLtexture() {
    glGenTextures(1, &this->address);
}

GLtexture::GLtexture(GLtexture &&texture)
    : address(texture.address)
{
    texture.address = 0;
}

void GLtexture::setData(GLint width, GLint height, char const *data) {
    this->bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
}

void GLtexture::bind() {
    glBindTexture(GL_TEXTURE_2D, this->address);
}

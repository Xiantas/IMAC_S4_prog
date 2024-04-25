#include "objects.h"

#include <glad/glad.h>
#include <span>
#include <optional>
#include "vertexData.h"

GLvbo::GLvbo() : vertexCount(0) {
    glGenBuffers(1, &this->address);
}

GLvbo::GLvbo(bool instanced)
    : address(0)
    , vertexCount(0)
{
    if (instanced) {
        glGenBuffers(1, &this->address);
    }
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
    , vboInstance(vaoType == VaoType::Instanced)
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

void GLvao::setVboInstance(std::span<glm::vec3> slice) {
    glBindBuffer(GL_ARRAY_BUFFER, this->vboInstance.address);
    glBufferData(GL_ARRAY_BUFFER, slice.size()*sizeof(glm::vec3), slice.data(), GL_DYNAMIC_DRAW);
    this->vboInstance.vertexCount = slice.size();
}

auto GLvao::getType() const -> VaoType {
    return this->vaoType;
}

void GLvao::draw() const {
    glBindVertexArray(this->address);
    if (this->vaoType == VaoType::Classic) {
        glDrawArrays(GL_TRIANGLES, 0, this->vboMain.getVertexCount());
    } else {
        glDrawArraysInstanced(
            GL_TRIANGLES, 0, 
            this->vboMain.getVertexCount(),
            this->vboInstance.getVertexCount()/2);
    }
}

GLtexture::GLtexture() : address(0) {}

GLtexture::GLtexture(GLtexture &&texture)
    : address(texture.address)
{
    texture.address = 0;
}

void GLtexture::setData(GLint width, GLint height, int nbChannels, unsigned char *data) {
    glGenTextures(1, &this->address);
    this->bind();

    GLint format = nbChannels == 3 ? GL_RGB : GL_RGBA;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void GLtexture::bind () const {
    glBindTexture(GL_TEXTURE_2D, this->address);
}

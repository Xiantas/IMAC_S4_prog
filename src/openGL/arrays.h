#pragma once

#include <glad/glad.h>

#include <string>
#include <filesystem>

#ifndef ARRAYS_H
#define ARRAYS_H

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>

GLuint createSingularBoidVBO();
GLuint createBoidsDisplacementVBO(const size_t N);
void setupVertexAttributes(GLuint singularBoidVBO, GLuint boidsDisplacementVBO);

#endif // ARRAYS_H

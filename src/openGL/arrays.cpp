#include "openGL/arrays.h"

#include <glad/glad.h>

#include <string>
#include "file_utils.h"

#include <iostream>

#include "arrays.h"

GLuint createSingularBoidVBO() {
    GLuint singularBoidVBO;
    glGenBuffers(1, &singularBoidVBO);
    glBindBuffer(GL_ARRAY_BUFFER, singularBoidVBO);
    GLfloat vertices[] = {
        0.03f,  0.f,
        -0.01f,  0.01f,
        -0.01f, -0.01f,
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return singularBoidVBO;
}

GLuint createBoidsDisplacementVBO(const size_t N) {
    std::vector<glm::vec2> drawData(2 * N);
    GLuint boidsDisplacementVBO;
    glGenBuffers(1, &boidsDisplacementVBO);
    glBindBuffer(GL_ARRAY_BUFFER, boidsDisplacementVBO);
    glBufferData(GL_ARRAY_BUFFER, drawData.size() * sizeof(glm::vec2), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return boidsDisplacementVBO;
}

void setupVertexAttributes(GLuint singularBoidVBO, GLuint boidsDisplacementVBO) {
    glBindBuffer(GL_ARRAY_BUFFER, singularBoidVBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, boidsDisplacementVBO);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
    glVertexAttribDivisor(1, 1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    glVertexAttribDivisor(2, 1);
}

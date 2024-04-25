#version 330 core

layout(location = 0) in vec3 aVertexPos;
layout(location = 1) in vec3 aVertexNor;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;

out vec3 fragPosition;
out vec3 fragNormal;
out vec2 fragTexCoords;

void main() {
    fragPosition = vec3(uMVMatrix * vec4(aVertexPos, 1.0));
    fragNormal = mat3(uNormalMatrix) * aVertexNor;
    fragTexCoords = aTexCoord;
    gl_Position = uMVPMatrix * vec4(aVertexPos, 1.0);
}

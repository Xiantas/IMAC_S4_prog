#version 330 core

layout(location = 0) in vec3 aVertexPos;
layout(location = 1) in vec3 aVertexNor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aBoidPos;
layout(location = 4) in vec3 aBoidDir;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;

out vec3 fragPosition;
out vec3 fragNormal;
out vec2 fragTexCoords;


vec2 complexProduct(vec2 a, vec2 b) {
    return vec2(a.x*b.x - a.y*b.y, a.x*b.y + a.y*b.x);
}

void main() {
    fragPosition = vec3(uMVMatrix * vec4(aVertexPos, 1.0));
    fragNormal = aVertexNor;
    fragTexCoords = aTexCoord;


    vec2 zRot = normalize(aBoidDir.xy);
    float newX = complexProduct(aBoidDir.xy, vec2(zRot.x, -zRot.y)).x;
    vec2 yRot = normalize(vec2(newX, aBoidDir.z));

    vec3 vertexPos = aVertexPos;

    vertexPos.xy = complexProduct(vertexPos.xy, zRot);
    vertexPos.xz = complexProduct(vertexPos.xz, yRot);

    gl_Position = uMVPMatrix * vec4(aBoidPos + vertexPos, 1.0);
}

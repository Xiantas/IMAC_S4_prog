#version 440 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aBoidPosition;
layout(location = 2) in vec2 aBoidDirection;

vec2 complexProduct(vec2 a, vec2 b) {
    return vec2(a.x*b.x - a.y*b.y, a.x*b.y + a.y*b.x);
}

void main() {
    vec2 normie = normalize(aBoidDirection);
    vec2 pos = aBoidPosition + complexProduct(aVertexPosition, normie);

    gl_Position = vec4(pos, 0.0, 1.0);
}

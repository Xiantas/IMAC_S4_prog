#version 330 core

in vec3 fragNormal;
in vec2 fragTexCoords;

out vec4 color;

uniform sampler2D image;

void main() {
    color = texture(image, fragTexCoords);
}

#version 330 core

uniform vec3 uColor;  // Ajout de la variable uniforme pour la couleur

in vec3 fragNormal;
out vec4 color;

void main() {
    color = vec4(uColor, 1.0);  // Utiliser la couleur passée par le programme
}

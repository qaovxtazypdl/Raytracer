#version 330

in vec3 vertexColour;
out vec4 fragColor;

void main() {
	fragColor = vec4(vertexColour, 1.0);
}

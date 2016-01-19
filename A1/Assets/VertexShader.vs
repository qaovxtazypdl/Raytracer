#version 330

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec4 colour;

in vec3 position;
out vec4 vertColor;

void main() {
	gl_Position = P * V * M * vec4(position, 1.0);
  vertColor = colour;
}

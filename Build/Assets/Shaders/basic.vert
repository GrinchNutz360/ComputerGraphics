#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec3 a_texcoord;

uniform float u_time;

//out vec3 ocolor;

void main() {

float frequency = 3.0;
float amplitude = 0.3;
vec3 position = a_position;

float offset = sin(u_time * frequency + position.y) * amplitude;
position.x += offset;

gl_Position = vec4(position, 1.0);
	//ocolor = color;
	gl_Position = vec4(a_position, 1.0);
}
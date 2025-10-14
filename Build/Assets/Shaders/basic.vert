#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_color;
//layout (location = 2) in vec3 a_texcoord;

uniform float u_time;

out vec3 v_color;
//out vec2 v_texcoord;

void main() {
v_color = a_color;

float frequency = 3.0;
float amplitude = 0.3;
vec3 position = a_position;

float offset = sin(u_time * frequency + position.y) * amplitude;
position.x += offset;

gl_Position = vec4(position, 1.0);
//v_texcoord = vec2(a_texcoord.x, a_texcoord.y);

}
#version 450 core

layout(location = 0) in vec3 _pos;
layout(location = 1) in vec2 _texpos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec2 texpos;

void main() {
    texpos = _texpos;
    gl_Position = proj * view * model * vec4(_pos, 1.0);
}

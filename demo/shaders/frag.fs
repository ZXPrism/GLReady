#version 450 core

in vec2 texpos;
out vec4 fragColor;

uniform vec3 color;

void main() {
    float eps = 0.01;

    if(texpos.x < eps || texpos.x > 1 - eps || texpos.y < eps || texpos.y > 1 - eps) {
        fragColor = vec4(0.0, 0.0, 0.0, 1.0);
    } else {
        fragColor = vec4(color, 1.0);
    }
}

#version 330 core

precision highp float;
precision highp int;

uniform vec4 u_color;
uniform vec3 u_minBoundary;
uniform vec3 u_maxBoundary;

in vec3 v_position;

out vec4 color;

void main()
{
    if (v_position.y < u_minBoundary.y) {
        color = vec4(0.0, -v_position.y * 0.1, 1.0, 1.0);
    } else {
        color = u_color;
    }
}

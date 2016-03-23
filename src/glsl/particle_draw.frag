#version 330 core

precision highp float;
precision highp int;

uniform vec3 u_minBoundary;
uniform vec3 u_maxBoundary;

in vec3 v_position;
in vec4 v_color;

out vec4 color;

void main()
{
    if (v_position.y < u_minBoundary.y) {
        color = vec4(0.0, -v_position.y * 0.1, 1.0, 1.0);
    } else {
        color = v_color;
    }
}

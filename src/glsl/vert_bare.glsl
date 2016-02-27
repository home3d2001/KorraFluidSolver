#version 150 core
#define POSITION_LOCATION 0

precision highp float;
precision highp int;
layout(std140, column_major) uniform;

uniform mat4 u_model;
uniform mat4 u_viewProj;

in vec3 a_position;

void main()
{
    vec4 position = u_viewProj * u_model * vec4(a_position, 1.0);
    gl_Position = position;
}

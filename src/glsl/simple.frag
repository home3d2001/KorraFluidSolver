#version 150 core

precision highp float;
precision highp int;
layout(std140, column_major) uniform;

out vec4 color;

void main()
{
    color = vec4(0.0, 1.0, 1.0, 1.0);
}

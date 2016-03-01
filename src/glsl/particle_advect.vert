#version 330 core

precision highp float;
precision highp int;
layout(std140, column_major) uniform;

uniform float u_time;
uniform vec3 u_accel;

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_velocity;
layout(location = 2) in float a_spawntime;

out vec3 v_position;
out vec3 v_velocity;
out float v_spawntime;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
    v_velocity = a_velocity + vec3(0.0, 0.01, 0.0);// u_time * u_accel + vec3(0.0, 0.1, 0.0);
    v_position = a_position + u_time * v_velocity + vec3(0.0, 0.1, 0.0);
    v_spawntime = a_spawntime;
}

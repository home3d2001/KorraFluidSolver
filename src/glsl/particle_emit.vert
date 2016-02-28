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
    float life_time = 5000.0;
    if (a_spawntime == 0.0 || (u_time - a_spawntime > life_time) || a_position.y < -0.5) {
        // Generate a new particle
        v_position = vec3(0.0, 0.0, 0.0);
        v_velocity = a_velocity;
        v_spawntime = u_time;
    } else {
        v_velocity = a_velocity + 0.01 * u_accel;
        v_position = a_position + 0.01 * v_velocity;
        v_spawntime = a_spawntime;
    }
}

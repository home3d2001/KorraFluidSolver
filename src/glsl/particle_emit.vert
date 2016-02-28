#version 150 core

precision highp float;
precision highp int;
layout(std140, column_major) uniform;

uniform float u_time;
uniform vec2 u_acceleration;

in vec2 a_position;
in vec2 a_velocity;
in float a_spawntime;
in float a_lifetime;
in float a_ID;

out vec2 v_position;
out vec2 v_velocity;
out float v_spawntime;
out float v_lifetime;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
    if (a_spawntime == 0.0 || (u_time - a_spawntime > a_lifetime) || a_position.y < -0.5) {
        // Generate a new particle
        v_position = vec2(0.0, 0.0);
        v_velocity = vec2(rand(vec2(a_ID, 0.0)) - 0.5, rand(vec2(a_ID, a_ID)));
        v_spawntime = u_time;
        v_lifetime = 5000.0;
    } else {
        v_velocity = a_velocity + 0.01 * u_acceleration;
        v_position = a_position + 0.01 * v_velocity;
        v_spawntime = a_spawntime;
        v_lifetime = a_lifetime;
    }
}

#version 150 core

precision highp float;
precision highp int;

uniform float u_time;
uniform vec2 u_acceleration;

in vec2 a_position;
in vec2 a_velocity;
in float a_spawntime;
in float a_lifetime;

void main()
{
    float deltaTime = u_time - a_spawntime;
    if (deltaTime < a_lifetime) {
        gl_Position = vec4(a_position, 0.0, 1.0);
    } else {
        gl_Position = vec4(-100.0, -100.0, 0.0, 1.0);
    }
    gl_PointSize = 10.0;
}

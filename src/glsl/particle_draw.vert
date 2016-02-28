#version 150 core

#define POSITION_LOCATION 0
#define VELOCITY_LOCATION 1
#define SPAWNTIME_LOCATION 2
#define LIFETIME_LOCATION 3

precision highp float;
precision highp int;

uniform float u_time;
uniform vec2 u_acceleration;

layout(location = POSITION_LOCATION) in vec2 a_position;
layout(location = VELOCITY_LOCATION) in vec2 a_velocity;
layout(location = SPAWNTIME_LOCATION) in float a_spawntime;
layout(location = LIFETIME_LOCATION) in float a_lifetime;

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

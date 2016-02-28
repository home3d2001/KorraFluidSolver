#version 330 core

precision highp float;
precision highp int;

uniform mat4 u_model;
uniform mat4 u_viewProj;
uniform float u_time;

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_velocity;
layout(location = 2) in float a_spawntime;

void main()
{
	float life_time = 5000.0;
    float deltaTime = u_time - a_spawntime;
    if (deltaTime < life_time) {
        gl_Position = u_viewProj * u_model * vec4(a_position, 1.0);
    } else {
        gl_Position = u_viewProj * u_model * vec4(-100.0, -100.0, 0.0, 1.0);
    }
}

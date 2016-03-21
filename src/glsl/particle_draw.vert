#version 330 core

precision highp float;
precision highp int;

uniform mat4 u_model;
uniform mat4 u_viewProj;
uniform float u_time;

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_velocity;
layout(location = 2) in float a_spawntime;

out vec3 v_position;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
	float r = rand(vec2(a_position.y, gl_VertexID));
	vec3 position = vec3(
		a_position.x,
		a_position.y - (u_time * u_time * u_time * u_time),
		a_position.z
		);
    gl_Position = u_viewProj * u_model * vec4(position, 1.0);
    v_position = position;
}

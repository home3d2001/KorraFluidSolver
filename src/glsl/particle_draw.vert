#version 330 core
#define POSITION_LOCATION 0
#define VELOCITY_LOCATION 1
#define SPAWNTIME_LOCATION 2
#define COLOR_LOCATION 3

precision highp float;
precision highp int;

uniform mat4 u_model;
uniform mat4 u_viewProj;
uniform float u_time;

layout(location = POSITION_LOCATION) in vec3 a_position;
layout(location = VELOCITY_LOCATION) in vec3 a_velocity;
layout(location = SPAWNTIME_LOCATION) in float a_spawntime;
layout(location = COLOR_LOCATION) in vec4 a_color;

out vec3 v_position;
out vec4 v_color;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
	vec3 position = vec3(
		a_position.x,
		a_position.y,// - (u_time * u_time * u_time * u_time),
		a_position.z
		);
    gl_Position = u_viewProj * u_model * vec4(position, 1.0);
    v_position = position;
    v_color = a_color;
}

#version 330 core 
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aColor;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 outColor;
out vec2 outUV;

void main()
{
	outColor = aColor;
	outUV = aUV;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
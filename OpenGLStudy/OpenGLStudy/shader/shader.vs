#version 330 core 
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;


out vec3 outColor;
out vec2 outUV;

void main()
{
	outColor = aColor;
	outUV = aUV;
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
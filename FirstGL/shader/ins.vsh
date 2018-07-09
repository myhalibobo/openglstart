#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aOffset;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fColor;

uniform vec2 offsets[100];

void main() {
	fColor = aColor;
	vec2 pos = aPos * (gl_InstanceID / 100.0);
	gl_Position = vec4(pos + aOffset,0.0f, 1.0f);
}

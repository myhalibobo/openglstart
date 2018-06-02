#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec4 outColor;
out vec2 textCoord;

uniform mat4 trans;

void main() {
	gl_Position = trans * vec4(aPos , 1.0f);
	//gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
	outColor = vec4(aColor, 1.0f);
	textCoord = aTexCoord;
}
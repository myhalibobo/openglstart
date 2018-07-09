#version 330 core
out vec4 FragColor;

in vec3 fColor;
in vec2 texCoord;

uniform sampler2D samTextrue;

void main(){
	FragColor = texture(samTextrue, texCoord);
}

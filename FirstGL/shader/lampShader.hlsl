#version 330 core
out vec4 FragColor;
in vec4 outColor;
in vec2 textCoord;

uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;

void main(){
	FragColor = vec4(1.0);
}

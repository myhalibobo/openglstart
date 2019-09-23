#version 330 core
out vec4 FragColor;

in vec3 outColor;
in vec2 outUV;

uniform sampler2D outTexture1;
uniform sampler2D outTexture2;

void main(){
    FragColor = vec4(1.0);
}

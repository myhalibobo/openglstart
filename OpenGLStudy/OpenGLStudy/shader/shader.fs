#version 330 core
out vec4 FragColor;

in vec3 outColor;
in vec2 outUV;

uniform sampler2D outTexture1;
uniform sampler2D outTexture2;

void main(){
	FragColor = mix(texture(outTexture1, outUV), texture(outTexture2, outUV), 0.2f);
	//FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
}
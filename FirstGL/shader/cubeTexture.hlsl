#version 330 core
out vec4 FragColor;


in vec3 Normal;
in vec3 Position;
in vec2 Texture;

uniform samplerCube skyCube;
uniform sampler2D cubeTexture;

uniform vec3 cameraPos;

void main(){
	vec3 I = normalize(Position - cameraPos);
	vec3 R = reflect(I, normalize(Normal));
	vec3 skyColor = texture(skyCube, R).rgb;
	FragColor = vec4(texture(skyCube, R).rgb, 1.0);
}

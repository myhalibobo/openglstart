#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D texture_0;



void main(){
	vec4 color = texture(texture_0, TexCoords);
	if (color.a < 0.1)
		discard;
	FragColor = color;
}

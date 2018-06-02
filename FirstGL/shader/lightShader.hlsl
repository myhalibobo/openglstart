#version 330 core
out vec4 FragColor;
in vec4 outColor;
in vec2 textCoord;

uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;

void main(){
	//FragColor = texture(ourTexture0 , textCoord);
	FragColor = mix(texture(ourTexture0, textCoord), texture(ourTexture1, textCoord), 0.2) * outColor;
	//FragColor = mix(texture(ourTexture0, textCoord), texture(ourTexture1, vec2(textCoord.x, textCoord.y)), 0.2);
}

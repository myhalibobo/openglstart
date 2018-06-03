#version 330 core
out vec4 FragColor;
in vec4 outColor;
in vec2 textCoord;

uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main(){
	FragColor = vec4(lightColor * objectColor, 1.0f);
	//FragColor = texture(ourTexture0 , textCoord);
	//FragColor = mix(texture(ourTexture0, textCoord), texture(ourTexture1, textCoord), 0.2) * outColor;
	//FragColor = mix(texture(ourTexture0, textCoord), texture(ourTexture1, vec2(textCoord.x, textCoord.y)), 0.2);
//FragColor = vec4(1);
}

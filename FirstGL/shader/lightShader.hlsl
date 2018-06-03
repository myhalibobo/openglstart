#version 330 core
out vec4 FragColor;

in vec4 outColor;
in vec2 textCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main(){
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;

	vec3 result = (ambient + diffuse) * objectColor;
	
	vec3 Temp = Normal;
	/*if (Temp.x < 0)
		Temp.x = 1;
	if (Temp.y < 0)
		Temp.y = 1;
	if (Temp.z < 0)
		Temp.z = 1;*/
	if (Temp.x == 0 && Temp.y == 0 && Temp.z == 0) {
		Temp = vec3(0.5, 0.5, 1);
	}
	FragColor = outColor;//vec4(vec3(outColor), 1.0);

	//FragColor = texture(ourTexture0 , textCoord);
	//FragColor = mix(texture(ourTexture0, textCoord), texture(ourTexture1, textCoord), 0.2) * outColor;
	//FragColor = mix(texture(ourTexture0, textCoord), texture(ourTexture1, vec2(textCoord.x, textCoord.y)), 0.2);
	//FragColor = vec4(1);
}

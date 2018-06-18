#version 330 core
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Lamp {
	vec3 position;

	vec3 direction;
	float cutoff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

out vec4 FragColor;

in vec2 textCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;

uniform Lamp lamp;
uniform Material material;


void main(){
	

	//float theta = dot(lightDir, normalize(-lamp.direction));

	//if (theta > lamp.cutoff) {
		// ambient
		vec3 ambient = lamp.ambient * texture(material.diffuse, textCoord).rgb;

		// diffuse 
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(lamp.position - FragPos);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = lamp.diffuse * diff * texture(material.diffuse, textCoord).rgb;

		// specular
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = lamp.specular * spec * texture(material.specular, textCoord).rgb;

		float distance = length(lamp.position - FragPos);
		float attenuation = 1.0 / (lamp.constant + lamp.linear * distance + lamp.quadratic * (distance * distance));

		ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;

		float theta = dot(lightDir, normalize(-lamp.direction));
		float epsilon = lamp.cutoff - lamp.outerCutOff;
		float intensity = clamp((theta - lamp.outerCutOff) / epsilon, 0.0, 1.0);

		diffuse *= intensity;
		specular *= intensity;

		vec3 result = ambient + diffuse + specular;
		FragColor = vec4(result, 1.0);
	//}
	//else {
	//	FragColor = vec4(lamp.ambient * texture(material.diffuse, textCoord).rgb, 1.0);
	//}
}

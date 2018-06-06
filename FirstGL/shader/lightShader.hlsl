#version 330 core
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Lamp {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

in vec2 textCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;

uniform Lamp lamp;
uniform Material material;


void main(){
	// ambient
	vec3 ambient = lamp.ambient * texture(material.diffuse , textCoord).rgb;

	// diffuse 
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(-lamp.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = lamp.diffuse * diff * texture(material.diffuse , textCoord).rgb;

	// specular
	float specularStrength = 0.9;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 specular = lamp.specular * spec * texture(material.specular, textCoord).rgb;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result , 1.0);

}

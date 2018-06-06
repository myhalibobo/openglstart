#version 330 core
struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Lamp {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

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
uniform vec3 viewPos;


uniform Lamp lamp;
uniform Material material;


void main(){
	// ambient
	vec3 ambient = material.ambient * lamp.ambient;

	// diffuse 
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lamp.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = (diff * material.diffuse) * lamp.diffuse;

	// specular
	float specularStrength = 0.9;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = (material.specular * spec) * lamp.specular;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result , 1.0);

}

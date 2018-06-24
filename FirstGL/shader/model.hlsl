#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;

struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float shininess;
};

uniform PointLight pointLight;


vec3 CalcSpotLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	//漫反射
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0);
	//反射
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), light.shininess);
	//合并
	vec3 ambient = light.ambient * texture(texture_diffuse1, TexCoords).rgb;
	vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, TexCoords).rgb;
	vec3 specular = light.specular* spec * vec3(texture(texture_diffuse1, TexCoords));
	//衰减
	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	//再合并
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;


	return (ambient + diffuse + specular);
}

void main()
{

	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 result = CalcSpotLight(pointLight, norm, FragPos, viewDir);
	FragColor = vec4(result, 1.0);
	//// ambient
	//vec3 ambient = pointLight.ambient * texture(texture_diffuse1, TexCoords).rgb;

	//// diffuse 
	//vec3 norm = normalize(Normal);
	//vec3 lightDir = normalize(pointLight.position - FragPos);
	//float diff = max(dot(norm, lightDir), 0.0);
	//vec3 diffuse = pointLight.diffuse * diff * texture(texture_diffuse1, TexCoords).rgb;

	//// specular
	//vec3 viewDir = normalize(viewDir - FragPos);
	//vec3 reflectDir = reflect(-lightDir, norm);
	//float spec = pow(max(dot(viewDir, reflectDir), 0.0), pointLight.shininess);
	//vec3 specular = pointLight.specular * spec * texture(texture_diffuse1, TexCoords).rgb;

	//vec3 reflectDir = reflect(-lightDir, Normal);
	//float spec = pow(max(dot(normalize(viewDir - FragPos), reflectDir), 0.0), pointLight.shininess);
	//vec3 specular = pointLight.specular* spec	* texture(texture_diffuse1, TexCoords).rgb;

	//// attenuation
	//float distance = length(pointLight.position - FragPos);
	//float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));

	//ambient *= attenuation;
	//diffuse *= attenuation;
	//specular *= attenuation;

	//vec3 result = ambient + diffuse;// +specular;
	//FragColor = vec4(result, 1.0);
}
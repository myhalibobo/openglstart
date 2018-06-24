#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewDir;
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

void main()
{
	////ambient
	//vec3 ambient = pointLight.ambient * texture(texture_diffuse1, TexCoords).rgb;
	////diffuse
	//vec3 lightDir = normalize(pointLight.position - FragPos);
	//vec3 diffuse = max(dot(lightDir, Normal) ,0) * pointLight.diffuse *  texture(texture_diffuse1, TexCoords).rgb;
	////spacular
	//vec3 spacular = pow(max(dot(reflect(-lightDir, viewDir), normalize(viewDir - FragPos)) , 0), pointLight.shininess) * pointLight.spacular;

	//float distance = length(pointLight.position - FragPos);
	//float attenuation = 1.0f / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * distance * distance);

	//ambient *= attenuation;
	//diffuse *= attenuation;
	//spacular *= attenuation;

	vec3 norm = normalize(Normal);
	//漫反射
	vec3 lightDir = normalize(pointLight.position - FragPos);
	float diff = max(dot(norm, lightDir), 0);

	//反射
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 view_Dir = normalize(viewDir - FragPos);
	float spec = pow(max(dot(view_Dir, reflectDir), 0.0), pointLight.shininess);

	//合并
	vec3 ambient = pointLight.ambient * texture(texture_diffuse1, TexCoords).rgb;
	vec3 diffuse = pointLight.diffuse * diff * texture(texture_diffuse1, TexCoords).rgb;
	vec3 specular = pointLight.specular * spec	* texture(texture_diffuse1, TexCoords).rgb;

	//衰减
	float distance = length(pointLight.position - FragPos);
	float attenuation = 1.0f / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * distance * distance);
	//再合并
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result = ambient + diffuse + specular;
	FragColor = result;



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
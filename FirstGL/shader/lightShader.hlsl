#version 330 core
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct SpotLight {
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

struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct DirLight {
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

uniform Material material;

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform struct DirLight dirLight;

uniform struct SpotLight spotLight;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	//漫反射
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0);
	//反射
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(normalize(viewDir - fragPos), reflectDir), 0.0), material.shininess);
	//合并
	vec3 ambient = light.ambient * texture(material.diffuse, textCoord).rgb;
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, textCoord).rgb;
	vec3 specular = light.specular* spec	* texture(material.specular, textCoord).rgb;

	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light , vec3 normal , vec3 fragPos , vec3 viewDir) {
	//漫反射
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0);
	//反射
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(normalize(viewDir - fragPos), reflectDir), 0.0), material.shininess);
	//合并
	vec3 ambient = light.ambient * texture(material.diffuse , textCoord).rgb;
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, textCoord).rgb;
	vec3 specular= light.specular* spec	* texture(material.specular, textCoord).rgb;
	//衰减
	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	//再合并
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	//漫反射
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0);
	//反射
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(normalize(viewDir - fragPos), reflectDir), 0.0), material.shininess);
	//合并
	vec3 ambient = light.ambient * texture(material.diffuse, textCoord).rgb;
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, textCoord).rgb;
	vec3 specular = light.specular* spec	* texture(material.specular, textCoord).rgb;
	//衰减
	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	//再合并
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutoff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	diffuse *= intensity;
	specular *= intensity;

	return (ambient + diffuse + specular);
}

void main(){
	vec3 result;
	result = CalcDirLight(dirLight, Normal, FragPos, viewPos);
	for (int i = 0; i < 4; i++) {
		result += CalcPointLight(pointLights[i], Normal, FragPos, viewPos);
	}
	result += CalcSpotLight(spotLight, Normal, FragPos, viewPos);
	FragColor = vec4(result, 1.0);
}

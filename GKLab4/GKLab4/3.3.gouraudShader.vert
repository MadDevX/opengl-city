#version 330 core
struct Material
{
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
};

struct DirLight 
{
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

struct PointLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight 
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 Color;
out vec2 TexCoords;

uniform float fogDistance;
uniform vec3 fogColor;

uniform Material material;

#define NUM_POINT_LIGHTS 2
#define NUM_SPOT_LIGHTS 2
#define NUM_DIR_LIGHTS 1
uniform PointLight pointLights[NUM_POINT_LIGHTS];
uniform DirLight dirLights[NUM_DIR_LIGHTS];
uniform SpotLight spotLights[NUM_SPOT_LIGHTS];

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPos;

uniform mat3 normal;

float near = 0.1f;
float far = 100.0f;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir); 
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir); 
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 vertexPos = vec3(model * vec4(aPos, 1.0f));
	vec3 norm = normalize(normal * aNormal);
	vec3 viewDir = normalize(viewPos - vertexPos);
	vec3 result = vec3(0.0f);
	for(int i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		result += CalcPointLight(pointLights[i], norm, vertexPos, viewDir);
	}
	for(int i = 0; i < NUM_SPOT_LIGHTS; i++)
	{
		result += CalcSpotLight(spotLights[i], norm, vertexPos, viewDir);
	}
	for(int i = 0; i < NUM_DIR_LIGHTS; i++)
	{
		result += CalcDirLight(dirLights[i], norm, viewDir);
	}
    TexCoords = aTexCoords;
	float depth = length(viewPos - vertexPos)/fogDistance;
    Color = mix(result, fogColor, depth);
    gl_Position = projection * view * vec4(vertexPos, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient, diffuse, specular;

	ambient  = light.ambient  * material.ambient;
	diffuse  = light.diffuse  * diff * material.diffuse;
	specular = light.specular * spec * material.specular;

    return (ambient + diffuse + specular);
}  

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float dist    = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * (dist * dist));    
    // combine results
    vec3 ambient, diffuse, specular;

	ambient  = light.ambient  * material.ambient;
	diffuse  = light.diffuse  * diff * material.diffuse;
	specular = light.specular * spec * material.specular;

    return (ambient + diffuse + specular) * attenuation;
} 

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient, diffuse, specular;

	ambient  = light.ambient  * material.ambient;
	diffuse  = light.diffuse  * diff * material.diffuse;
	specular = light.specular * spec * material.specular;

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
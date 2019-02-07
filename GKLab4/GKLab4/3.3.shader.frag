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

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

#define NUM_POINT_LIGHTS 2
#define NUM_SPOT_LIGHTS 2
#define NUM_DIR_LIGHTS 1

uniform float fogDistance;
uniform vec3 fogColor;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 viewPos;
uniform Material material;
uniform bool isTextured;
uniform PointLight pointLights[NUM_POINT_LIGHTS];
uniform DirLight dirLights[NUM_DIR_LIGHTS];
uniform SpotLight spotLights[NUM_SPOT_LIGHTS];

float near = 0.1f;
float far = 100f;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir); 
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir); 
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{    		
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	
	vec3 result = vec3(0.0f);
	for(int i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	}
	for(int i = 0; i < NUM_SPOT_LIGHTS; i++)
	{
		result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);
	}
	for(int i = 0; i < NUM_DIR_LIGHTS; i++)
	{
		result += CalcDirLight(dirLights[i], norm, viewDir);
	}

	float depth = min(LinearizeDepth(gl_FragCoord.z)/fogDistance, 1.0f);
    FragColor = vec4(mix(result, fogColor, depth), 1.0f);
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

	if(isTextured)
	{
		ambient  = light.ambient  * vec3(texture(texture_diffuse1, TexCoords));
		diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, TexCoords));
		specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
	}
	else
	{
		ambient  = light.ambient  * material.ambient;
		diffuse  = light.diffuse  * diff * material.diffuse;
		specular = light.specular * spec * material.specular;
	}

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

	if(isTextured)
	{
		ambient  = light.ambient  * vec3(texture(texture_diffuse1, TexCoords));
		diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, TexCoords));
		specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
	}
	else
	{
		ambient  = light.ambient  * material.ambient;
		diffuse  = light.diffuse  * diff * material.diffuse;
		specular = light.specular * spec * material.specular;
	}

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

	if(isTextured)
	{
		ambient  = light.ambient  * vec3(texture(texture_diffuse1, TexCoords));
		diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, TexCoords));
		specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
	}
	else
	{
		ambient  = light.ambient  * material.ambient;
		diffuse  = light.diffuse  * diff * material.diffuse;
		specular = light.specular * spec * material.specular;
	}

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
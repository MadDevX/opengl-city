#version 330 core
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Light
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

uniform Material material;
uniform Light light;
uniform float time;

void main()
{
	//AMBIENT
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

	//DIFFUSE
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * (diff * texture(material.diffuse, TexCoord).rgb);

	//SPECULAR
	vec3 viewDir = normalize(-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * (spec * texture(material.specular, TexCoord).rgb);

	//EMISSION
	vec3 emission = texture(material.emission, TexCoord).rgb;

	//RESULT
	vec3 result = ambient + diffuse + specular + emission;

    FragColor = vec4(result, 1.0f);
}
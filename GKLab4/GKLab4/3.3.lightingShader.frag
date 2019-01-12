#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;
in vec2 TexCoord;
  
uniform vec3 objectColor;
uniform vec3 lightColor;

uniform sampler2D ourTexture;

void main()
{
	//AMBIENT
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;

	//DIFFUSE
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;

	//SPECULAR
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
	vec3 specular = specularStrength * spec * lightColor;

	//RESULT
	vec3 result = (ambient + diffuse + specular) * objectColor;// * vec3(texture(ourTexture, TexCoord));

    FragColor = vec4(result, 1.0);
}
#version 330 core
out vec4 FragColor;

in vec3 Color;
in vec2 TexCoords;

uniform bool isTextured;
uniform sampler2D texture_diffuse1;


void main()
{  
	if(isTextured)
	{
		FragColor = vec4(Color, 1.0f) * texture(texture_diffuse1, TexCoords);
	}
	else
	{
		FragColor = vec4(Color, 1.0f);
	}
}
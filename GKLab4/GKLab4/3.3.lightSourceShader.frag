#version 330 core
out vec4 FragColor;

float near = 0.1f;
float far = 100.0f;

uniform float fogDistance;
uniform vec3 fogColor;
uniform vec3 lightColor;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
	float depth = min(LinearizeDepth(gl_FragCoord.z)/fogDistance, 1.0f);
    FragColor = vec4(mix(lightColor, fogColor, depth), 1.0f);
}
#version 330 core
out vec4 FragColor;

float near = 0.1f;
float far = 100f;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
	float depth = LinearizeDepth(gl_FragCoord.z)/far;
    FragColor = mix(vec4(1.0), vec4(0.3f, 0.3f, 0.3f, 1.0f), depth);
}
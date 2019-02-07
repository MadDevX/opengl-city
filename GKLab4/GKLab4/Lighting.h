#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <LearnOpenGL/Shader.h>

struct PointLight
{
	glm::vec3 position;
	
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;

	void SetLight(Shader shader, int index)
	{
		shader.setVec3("pointLights[" + std::to_string(index) + "].position", position);
		shader.setVec3("pointLights[" + std::to_string(index) + "].ambient", ambient);
		shader.setVec3("pointLights[" + std::to_string(index) + "].diffuse", diffuse);
		shader.setVec3("pointLights[" + std::to_string(index) + "].specular", specular);
		shader.setFloat("pointLights[" + std::to_string(index) + "].constant", constant);
		shader.setFloat("pointLights[" + std::to_string(index) + "].linear", linear);
		shader.setFloat("pointLights[" + std::to_string(index) + "].quadratic", quadratic);
	}
};

struct SpotLight
{
	glm::vec3 position;
	glm::vec3 direction;
	float cutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	void SetLight(Shader shader, int index)
	{
		shader.setVec3("spotLights[" + std::to_string(index) + "].position", position);
		shader.setVec3("spotLights[" + std::to_string(index) + "].direction", direction);
		shader.setFloat("spotLights[" + std::to_string(index) + "].cutOff", cutOff);
		shader.setFloat("spotLights[" + std::to_string(index) + "].outerCutOff", outerCutOff);
		shader.setFloat("spotLights[" + std::to_string(index) + "].constant", constant);
		shader.setFloat("spotLights[" + std::to_string(index) + "].linear", linear);
		shader.setFloat("spotLights[" + std::to_string(index) + "].quadratic", quadratic);
		shader.setVec3("spotLights[" + std::to_string(index) + "].ambient", ambient);
		shader.setVec3("spotLights[" + std::to_string(index) + "].diffuse", diffuse);
		shader.setVec3("spotLights[" + std::to_string(index) + "].specular", specular);
	}
};

struct DirectionalLight
{
	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	void SetLight(Shader shader, int index)
	{
		shader.setVec3("dirLights[" + std::to_string(index) + "].direction", direction);
		shader.setVec3("dirLights[" + std::to_string(index) + "].ambient", ambient);
		shader.setVec3("dirLights[" + std::to_string(index) + "].diffuse", diffuse);
		shader.setVec3("dirLights[" + std::to_string(index) + "].specular", specular);
	}
};
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <LearnOpenGL/Shader.h>
#include <LearnOpenGL/Model.h>

class Node
{
public:
	glm::mat4 modelMatrix;
	Model *model;

	Node(Model *model, glm::mat4 modelMatrix)
	{
		this->model = model;
		this->modelMatrix = modelMatrix;
	}

	void Move(glm::vec3 translation)
	{
		glm::vec4 vec(translation.x, translation.y, translation.z, 1.0f);
		glm::vec4 curTranslation(0.0f, 0.0f, 0.0f, 1.0f);
		curTranslation = modelMatrix * curTranslation;
		glm::mat4 localMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-curTranslation)) * modelMatrix;//glm::translate(modelMatrix, glm::vec3(-curTranslation));
		vec = localMatrix * vec;
		modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(vec)) * modelMatrix;
	}

	void Rotate(float angle, glm::vec3 axis)
	{
		modelMatrix = glm::rotate(modelMatrix, angle, axis);
	}

	void Draw(Shader shader)
	{
		shader.setMat4("model", modelMatrix);
		glm::mat3 normal = glm::transpose(glm::inverse(modelMatrix));
		shader.setMat3("normal", normal);

		model->Draw(shader);
	}
};

class LightNode
{
	glm::mat4 modelMatrix;

};
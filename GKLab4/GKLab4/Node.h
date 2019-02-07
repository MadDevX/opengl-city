#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <LearnOpenGL/Shader.h>
#include <LearnOpenGL/Model.h>
#include <vector>
#include "Lighting.h"


class Node
{
public:
	Node* parent;
	glm::mat4 modelMatrix;
	vector<Node*> children;

	void AttachChild(Node* node)
	{
		node->parent = this;
		children.push_back(node);
	}

	virtual void Draw(Shader shader, glm::mat4 parentMatrix = glm::mat4(1.0f))
	{
		glm::mat4 currentMatrix = parentMatrix * modelMatrix;
		DrawChildren(shader, currentMatrix);
	}

	void DrawChildren(Shader shader, glm::mat4 parentMatrix)
	{
		for (int i = 0; i < children.size(); i++)
		{
			children[i]->Draw(shader, parentMatrix);
		}
	}

	void Move(glm::vec3 translation)
	{
		glm::vec4 vec(translation.x, translation.y, translation.z, 1.0f);
		glm::vec4 curTranslation(0.0f, 0.0f, 0.0f, 1.0f);
		curTranslation = modelMatrix * curTranslation;
		glm::mat4 localMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-curTranslation)) * modelMatrix;
		vec = localMatrix * vec;
		modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(vec)) * modelMatrix;
	}

	void Rotate(float angle, glm::vec3 axis)
	{
		modelMatrix = glm::rotate(modelMatrix, angle, axis);
	}

	glm::mat4 GetGlobalModelMatrix()
	{
		if (parent == nullptr)
		{
			return modelMatrix;
		}
		else
		{
			return parent->GetGlobalModelMatrix() * modelMatrix;
		}
	}

	glm::vec3 GetWorldPosition()
	{
		return GetGlobalModelMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
};

class ModelNode : public Node
{
public:
	Model *model;

	ModelNode(Model *model, glm::mat4 modelMatrix)
	{
		this->model = model;
		this->modelMatrix = modelMatrix;
	}

	void Draw(Shader shader, glm::mat4 parentMatrix = glm::mat4(1.0f))
	{
		glm::mat4 currentMatrix = parentMatrix * modelMatrix;
		shader.setMat4("model", currentMatrix);
		glm::mat3 normal = glm::transpose(glm::inverse(currentMatrix));
		shader.setMat3("normal", normal);
		model->Draw(shader);
		DrawChildren(shader, currentMatrix);
	}
};

class PointLightNode : public Node
{
public:
	PointLight light;

	PointLightNode(PointLight light, glm::mat4 modelMatrix)
	{
		this->light = light;
		this->modelMatrix = modelMatrix;
	}

	void UpdateLight(Shader shader, int index)
	{
		PointLight temp = light;
		temp.position = GetGlobalModelMatrix() * glm::vec4(light.position, 1.0f);
		temp.SetLight(shader, index);
	}
};

class SpotLightNode : public Node
{
public:
	SpotLight light;

	SpotLightNode(SpotLight light, glm::mat4 modelMatrix)
	{
		this->light = light;
		this->modelMatrix = modelMatrix;
	}

	void UpdateLight(Shader shader, int index)
	{
		SpotLight temp = light;
		temp.position = GetGlobalModelMatrix() * glm::vec4(light.position, 1.0f);
		temp.direction = glm::mat3(glm::transpose(glm::inverse(GetGlobalModelMatrix()))) * light.direction;
		temp.SetLight(shader, index);
	}
};
#pragma once

#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <LearnOpenGL/Mesh.h>
#include <LearnOpenGL/Shader.h>

struct Face
{
	Vertex vertices[3];

	Face(glm::vec3 vertices[3])
	{
		for (int i = 0; i < 3; i++)
		{
			this->vertices[i].Position = vertices[i];
		}
	}
};

struct Sphere
{
	Vertex *vertices;
	Material material;
	float radius;
	int currentVertex;
	int vertexCount;

	Sphere(float radius, int subdivisions, Material material)
	{
		this->radius = radius;
		this->material = material;
		vertexCount = 8 * 3 * pow(4, subdivisions);
		vertices = new Vertex[vertexCount];
		currentVertex = 0;

		glm::vec3 verts[3];
		verts[0] = glm::vec3(radius, 0.0f, 0.0f);
		verts[1] = glm::vec3(0.0f, radius, 0.0f);
		verts[2] = glm::vec3(0.0f, 0.0f, radius);
		Subdivide(Face(verts), subdivisions);

		verts[0] = glm::vec3(-radius, 0.0f, 0.0f);
		verts[1] = glm::vec3(0.0f, radius, 0.0f);
		verts[2] = glm::vec3(0.0f, 0.0f, radius);
		Subdivide(Face(verts), subdivisions);

		verts[0] = glm::vec3(radius, 0.0f, 0.0f);
		verts[1] = glm::vec3(0.0f, -radius, 0.0f);
		verts[2] = glm::vec3(0.0f, 0.0f, radius);
		Subdivide(Face(verts), subdivisions);

		verts[0] = glm::vec3(radius, 0.0f, 0.0f);
		verts[1] = glm::vec3(0.0f, radius, 0.0f);
		verts[2] = glm::vec3(0.0f, 0.0f, -radius);
		Subdivide(Face(verts), subdivisions);

		verts[0] = glm::vec3(-radius, 0.0f, 0.0f);
		verts[1] = glm::vec3(0.0f, -radius, 0.0f);
		verts[2] = glm::vec3(0.0f, 0.0f, radius);
		Subdivide(Face(verts), subdivisions);

		verts[0] = glm::vec3(-radius, 0.0f, 0.0f);
		verts[1] = glm::vec3(0.0f, radius, 0.0f);
		verts[2] = glm::vec3(0.0f, 0.0f, -radius);
		Subdivide(Face(verts), subdivisions);

		verts[0] = glm::vec3(radius, 0.0f, 0.0f);
		verts[1] = glm::vec3(0.0f, -radius, 0.0f);
		verts[2] = glm::vec3(0.0f, 0.0f, -radius);
		Subdivide(Face(verts), subdivisions);

		verts[0] = glm::vec3(-radius, 0.0f, 0.0f);
		verts[1] = glm::vec3(0.0f, -radius, 0.0f);
		verts[2] = glm::vec3(0.0f, 0.0f, -radius);
		Subdivide(Face(verts), subdivisions);

		setupMesh();
	}

	void Draw(Shader shader)
	{
		shader.setBool("isTextured", false);

		shader.setVec3("material.ambient", material.ambient);
		shader.setVec3("material.diffuse", material.diffuse);
		shader.setVec3("material.specular", material.specular);
		shader.setFloat("material.shininess", material.shininess);

		// draw mesh
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
		glBindVertexArray(0);
	}

	~Sphere()
	{
		delete[] vertices;
		vertices = nullptr;
	}

private:
	unsigned int VAO, VBO;
	void Subdivide(Face face, int divisions)
	{
		if (divisions <= 0)
		{
			for (int i = 0; i < 3; i++)
			{
				face.vertices[i].Position = glm::normalize(face.vertices[i].Position) * radius;
				face.vertices[i].Normal = glm::normalize(face.vertices[i].Position);
				if (vertices)
				{
					vertices[currentVertex++] = face.vertices[i];
				}
			}
		}
		else
		{
			glm::vec3 midVertices[3];
			for (int i = 0; i < 3; i++)
			{
				midVertices[i] = (face.vertices[i].Position + face.vertices[(i + 1) % 3].Position) * 0.5f;
			}
			Subdivide(Face(midVertices), divisions - 1);
			glm::vec3 verts[3];

			verts[0] = face.vertices[0].Position;
			verts[1] = midVertices[0];
			verts[2] = midVertices[2];
			Subdivide(Face(verts), divisions - 1);

			verts[0] = midVertices[0];
			verts[1] = face.vertices[1].Position;
			verts[2] = midVertices[1];
			Subdivide(Face(verts), divisions - 1);

			verts[0] = midVertices[2];
			verts[1] = midVertices[1];
			verts[2] = face.vertices[2].Position;
			Subdivide(Face(verts), divisions - 1);
		}
	}

	void setupMesh()
	{
		// create buffers/arrays
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		// load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		// set the vertex attribute pointers
		// vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		// vertex tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
		// vertex bitangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

		glBindVertexArray(0);
	}
};
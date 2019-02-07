#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <LearnOpenGL/Shader.h>
#include <LearnOpenGL/Camera.h>
#include <LearnOpenGL/Model.h>
#include "Node.h"
#include "Lighting.h"
#include "SphereGenerator.h"

GLFWwindow* initOpenGLContext();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window, Node *car, SpotLightNode *nodes);
void updateCameras(Node *car);
unsigned int loadTexture(char const * path);

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

// camera
int activeCamera;
Camera cameras[3] =
{
	Camera(glm::vec3(0.0f, 0.0f, 3.0f)),
	Camera(glm::vec3(0.0f, 0.0f, 0.0f)),
	Camera(glm::vec3(-1.0f, 0.0f, 10.0f))
};

float lastX = WINDOW_WIDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f;

int day;
bool wasPressed = false;

float fogDistance = 20.0f;
float fogAdjustmentSpeed = 5.0f;
float carSpeed = 10.0f;
float carRotateSpeed = 1.25f;
float maxAngle = 0.75f;
float currentAngle = 0.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
	GLFWwindow* window = initOpenGLContext();

	Shader modelShader("3.3.shader.vert", "3.3.shader.frag");
	Shader lightingShader("3.3.shader.vert", "3.3.lightingShader.frag");
	Shader lightSourceShader("3.3.shader.vert", "3.3.lightSourceShader.frag");

	float vertices[] =
	{
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};
	Model cityModel("Resources/city/cityS.obj");
	Model carModel("Resources/Car/Chevrolet_Camaro_SS_Low.obj");

	ModelNode city(&cityModel, glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f)), glm::vec3(0.75f, 0.75f, 0.75f)));
	ModelNode car(&carModel, glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.65f, 0.0f)), glm::vec3(0.25f, 0.25f, 0.25f)));
	//car.AttachChild(&ModelNode(&carModel, glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 10.0f)), glm::vec3(0.5f, 0.5f, 0.5f))));

	Material sphereMat{ glm::vec3(0.0f, 0.5f, 1.0f), glm::vec3(0.0f, 0.5f, 1.0f), glm::vec3(1.0f), 64.0f };
	Sphere mySphere(0.3f, 5, sphereMat);
	GeometryNode sphereNode(&mySphere, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

	PointLight pointLights[] =
	{
		{glm::vec3(0.7f,  1.2f,  2.0f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0.3f, 0.3f, 0.3f), 1.0f, 0.09f, 0.032f},
		{glm::vec3(0.7f,  2.5f,  -5.0f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0.3f, 0.3f, 0.3f), 1.0f, 0.09f, 0.032f}
	};

	SpotLight spotLights[] =
	{
		{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(25.0f)), 1.0f, 0.09f, 0.032f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)},
		{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(25.0f)), 1.0f, 0.09f, 0.032f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)}
	};

	DirectionalLight dirLights[]
	{
		{glm::vec3(0.4f, -0.5f, -0.2f), glm::vec3(0.05f, 0.05f, 0.07f), glm::vec3(0.05f, 0.05f, 0.15f), glm::vec3(0.3f, 0.3f, 0.3f)},
		{glm::vec3(-0.4f, -0.5f, 0.2f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.5f, 0.5f, 0.4f), glm::vec3(0.5f, 0.5f, 0.5f)}
	};

	glm::vec3 fogColors[]
	{
		glm::vec3(0.05f, 0.05f, 0.05f),
		glm::vec3(0.45f, 0.45f, 0.4f)
	};

	SpotLightNode spotLightNodes[] =
	{
		SpotLightNode(spotLights[0], glm::translate(glm::mat4(1.0f), glm::vec3(-1.1f, -0.2f, 4.5f))),
		SpotLightNode(spotLights[1], glm::translate(glm::mat4(1.0f), glm::vec3(1.1f, -0.2f, 4.5f))),
	};
	car.AttachChild(&spotLightNodes[0]);
	car.AttachChild(&spotLightNodes[1]);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int texture = loadTexture("container2.png");
	unsigned int specularMap = loadTexture("container2_specular.png");
	unsigned int emissionMap = loadTexture("matrix.jpg");

	lightingShader.use();
	lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.specular", 1);
	lightingShader.setInt("material.emission", 2);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window, &car, spotLightNodes);
		glfwPollEvents();

		updateCameras(&car);

		//rendering
		glClearColor(fogColors[day].x, fogColors[day].y, fogColors[day].z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		modelShader.use();
		modelShader.setFloat("fogDistance", fogDistance);
		modelShader.setVec3("fogColor", fogColors[day]);

		glm::mat4 view = cameras[activeCamera].GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(cameras[activeCamera].Zoom), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);
		modelShader.setMat4("projection", projection);
		modelShader.setMat4("view", view);

		{
			modelShader.setVec3("viewPos", cameras[activeCamera].Position);

			dirLights[day].SetLight(modelShader, 0);
			for (int i = 0; i < 2; i++)
			{
				pointLights[i].SetLight(modelShader, i);
				spotLightNodes[i].UpdateLight(modelShader, i);
			}
			city.Draw(modelShader);
			car.Draw(modelShader);
			sphereNode.Draw(modelShader);
		}

		glBindVertexArray(lightVAO);
		{
			lightSourceShader.use();
			lightSourceShader.setMat4("view", view);
			lightSourceShader.setMat4("projection", projection);

			for (int i = 0; i < 2; i++)
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, pointLights[i].position);
				model = glm::scale(model, glm::vec3(0.2f));
				lightSourceShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}
		//	// directional light
		//	lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		//	lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		//	lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		//	lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}

void configureGLFWContext()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void carInput(GLFWwindow *window, Node *car, SpotLightNode *spotLightNodes)
{
	bool canRotate = false;
	float rotateDir;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
	{
		car->Move(glm::vec3(0.0f, 0.0f, deltaTime * carSpeed));
		canRotate = !canRotate;
		rotateDir = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	{
		car->Move(glm::vec3(0.0f, 0.0f, -deltaTime * carSpeed));
		canRotate = !canRotate;
		rotateDir = -1.0f;
	}
	if (canRotate)
	{
		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
			car->Rotate(deltaTime * carRotateSpeed, glm::vec3(0.0f, rotateDir, 0.0f));
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
			car->Rotate(-deltaTime * carRotateSpeed, glm::vec3(0.0f, rotateDir, 0.0f));
	}

	//rotate spotlights
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		currentAngle += deltaTime;
		if (currentAngle >= maxAngle)
		{
			currentAngle = maxAngle;
		}
		else
		{
			for (int i = 0; i < 2; i++)
				spotLightNodes[i].modelMatrix = glm::rotate(spotLightNodes[i].modelMatrix, deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
		}
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		currentAngle -= deltaTime;
		if (currentAngle <= -maxAngle)
		{
			currentAngle = -maxAngle;
		}
		else
		{
			for (int i = 0; i < 2; i++)
				spotLightNodes[i].modelMatrix = glm::rotate(spotLightNodes[i].modelMatrix, -deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
		}
	}
}

void cameraInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		activeCamera = 0;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		activeCamera = 1;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		activeCamera = 2;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	if (activeCamera == 0)
	{
		float cameraSpeed = 1.0f * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameras[activeCamera].ProcessKeyboard(FORWARD, cameraSpeed);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameras[activeCamera].ProcessKeyboard(BACKWARD, cameraSpeed);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameras[activeCamera].ProcessKeyboard(LEFT, cameraSpeed);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameras[activeCamera].ProcessKeyboard(RIGHT, cameraSpeed);
	}
}

void settingsInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		fogDistance = fminf(100.0f, fogDistance + deltaTime * fogAdjustmentSpeed);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		fogDistance = fmaxf(1.0f, fogDistance - deltaTime * fogAdjustmentSpeed);

	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
	{
		if (wasPressed == false)
		{
			day = 1 - day;
			wasPressed = true;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_N) == GLFW_RELEASE)
	{
		wasPressed = false;
	}
}
void processInput(GLFWwindow *window, Node *car, SpotLightNode *spotLightNodes)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	settingsInput(window);

	cameraInput(window);

	carInput(window, car, spotLightNodes);
}

unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

GLFWwindow* createWindow()
{
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "City", NULL, NULL);
	if (window == NULL)
	{
		glfwTerminate();
		throw - 1;
	}
	glfwMakeContextCurrent(window);

	return window;
}

GLFWwindow* initOpenGLContext()
{
	glfwInit();
	configureGLFWContext();

	GLFWwindow* window;

	try
	{
		window = createWindow();
	}
	catch (int)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		exit(EXIT_FAILURE);
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(EXIT_FAILURE);
	}

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	return window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos;

	lastX = (float)xpos;
	lastY = (float)ypos;

	if (activeCamera == 0)
		cameras[activeCamera].ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	cameras[activeCamera].ProcessMouseScroll((float)yoffset);
}

void updateCameras(Node *car)
{
	glm::mat4 offset = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 5.0f, -15.0f));
	glm::vec4 carPos = car->modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	cameras[1].Position = car->modelMatrix * offset * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	cameras[1].Front = glm::vec3(carPos.x, carPos.y + 0.5f, carPos.z) - cameras[1].Position;

	cameras[2].Front = glm::vec3(carPos.x, carPos.y + 0.25f, carPos.z) - cameras[2].Position;
}
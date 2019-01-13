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

GLFWwindow* initOpenGLContext();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(char const * path);

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = WINDOW_WIDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f;
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

	glm::vec3 cubePositions[] = 
	{
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	Model ourModel("Resources/nanosuit/nanosuit.obj");

	// positions of the point lights
	glm::vec3 pointLightPositions[] = 
	{
		glm::vec3(0.7f,  1.2f,  2.0f),
		glm::vec3(-2.5f, -2.5f, -3.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

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

		processInput(window);
		glfwPollEvents();

		//rendering
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		modelShader.use();
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);
		modelShader.setMat4("projection", projection);
		modelShader.setMat4("view", view);

		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
			model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
			modelShader.setMat4("model", model);
			glm::mat3 normal = glm::transpose(glm::inverse(model));
			modelShader.setMat3("normal", normal);
			modelShader.setVec3("viewPos", camera.Position);
			modelShader.setFloat("material.shininess", 16.0f);

			modelShader.setVec3("pointLights[0].position", pointLightPositions[0]);
			modelShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
			modelShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
			modelShader.setVec3("pointLights[0].specular", 0.3f, 0.3f, 0.3f);
			modelShader.setFloat("pointLights[0].constant", 1.0f);
			modelShader.setFloat("pointLights[0].linear", 0.09f);
			modelShader.setFloat("pointLights[0].quadratic", 0.032f);

			modelShader.setVec3("pointLights[1].position", pointLightPositions[1]);
			modelShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
			modelShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
			modelShader.setVec3("pointLights[1].specular", 0.3f, 0.3f, 0.3f);
			modelShader.setFloat("pointLights[1].constant", 1.0f);
			modelShader.setFloat("pointLights[1].linear", 0.09f);
			modelShader.setFloat("pointLights[1].quadratic", 0.032f);

			ourModel.Draw(modelShader);
		}

		glBindVertexArray(lightVAO);
		{
			lightSourceShader.use();
			lightSourceShader.setMat4("view", view);
			lightSourceShader.setMat4("projection", projection);

			for (int i = 0; i < 2; i++)
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, pointLightPositions[i]);
				model = glm::scale(model, glm::vec3(0.2f));
				lightSourceShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

		//glBindVertexArray(VAO);
		//{
		//	lightingShader.use();
		//	lightingShader.setMat4("view", view);
		//	lightingShader.setMat4("projection", projection);
		//	lightingShader.setVec3("viewPos", camera.Position);

		//	lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		//	lightingShader.setFloat("material.shininess", 32.0f);

		//	// directional light
		//	lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		//	lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		//	lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		//	lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		//	// point light 1
		//	lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		//	lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		//	lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		//	lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		//	lightingShader.setFloat("pointLights[0].constant", 1.0f);
		//	lightingShader.setFloat("pointLights[0].linear", 0.09f);
		//	lightingShader.setFloat("pointLights[0].quadratic", 0.032f);
		//	// point light 2
		//	lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		//	lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		//	lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		//	lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		//	lightingShader.setFloat("pointLights[1].constant", 1.0f);
		//	lightingShader.setFloat("pointLights[1].linear", 0.09f);
		//	lightingShader.setFloat("pointLights[1].quadratic", 0.032f);
		//	// point light 3
		//	lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		//	lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		//	lightingShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		//	lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		//	lightingShader.setFloat("pointLights[2].constant", 1.0f);
		//	lightingShader.setFloat("pointLights[2].linear", 0.09f);
		//	lightingShader.setFloat("pointLights[2].quadratic", 0.032f);
		//	// point light 4
		//	lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
		//	lightingShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		//	lightingShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		//	lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		//	lightingShader.setFloat("pointLights[3].constant", 1.0f);
		//	lightingShader.setFloat("pointLights[3].linear", 0.09f);
		//	lightingShader.setFloat("pointLights[3].quadratic", 0.032f);
		//	// spotLight
		//	lightingShader.setVec3("spotLight.position", camera.Position);
		//	lightingShader.setVec3("spotLight.direction", camera.Front);
		//	lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		//	lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		//	lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		//	lightingShader.setFloat("spotLight.constant", 1.0f);
		//	lightingShader.setFloat("spotLight.linear", 0.09f);
		//	lightingShader.setFloat("spotLight.quadratic", 0.032f);
		//	lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		//	lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_2D, texture);
		//	glActiveTexture(GL_TEXTURE1);
		//	glBindTexture(GL_TEXTURE_2D, specularMap);
		//	//glActiveTexture(GL_TEXTURE2);
		//	//glBindTexture(GL_TEXTURE_2D, emissionMap);

		//	for (unsigned int i = 0; i < 10; i++)
		//	{
		//		glm::mat4 model(1.0f);
		//		model = glm::translate(model, cubePositions[i]);
		//		float angle = 20.0f * (i);
		//		model = glm::rotate(model, (float)glfwGetTime()*glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		//		lightingShader.setMat4("model", model);
		//		glm::mat3 normal = glm::transpose(glm::inverse(model));
		//		lightingShader.setMat3("normal", normal);

		//		glDrawArrays(GL_TRIANGLES, 0, 36);
		//	}
		//}


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

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true); 
	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
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

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}

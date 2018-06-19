#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Camera.h"
#include <iostream>
#include "CELLMath.hpp"
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
const float winW = 800;
const float winH = 600;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool firstMouse = true;
float lastX = winW / 2.0f;
float lastY = winH / 2.0f;
unsigned int _VBO, lightVAO, _EBO, lampVAO;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}


void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

unsigned int createTexture(const char *texturePath , bool isPng) {
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//warp
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, heigth, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(texturePath, &width, &heigth, &nrChannels, 0);
	if (data) {
		if(isPng)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, heigth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, heigth, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);
	}else{
		cout << "FAILD TO LOAD TEXTURE" << endl;
	}

	stbi_image_free(data);

	return texture;
}

void initVAO() {
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f ,	 0.0f, 0.0f,      0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f ,	 1.0f, 0.0f,	  0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 1.0f ,	 1.0f, 1.0f,	  0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 1.0f ,	 1.0f, 1.0f,	  0.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 1.0f ,	 0.0f, 1.0f,	  0.0f, 0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f ,	 0.0f, 0.0f,      0.0f, 0.0f, -1.0f,

		-0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 1.0f ,	 0.0f, 0.0f,	  0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 1.0f ,	 1.0f, 0.0f,	  0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 1.0f ,	 1.0f, 1.0f,	  0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 1.0f ,	 1.0f, 1.0f,	  0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 1.0f ,	 0.0f, 1.0f,	  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 1.0f ,	 0.0f, 0.0f,	  0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f ,	 1.0f, 0.0f,	  -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f ,	 1.0f, 1.0f,	  -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 1.0f ,	 0.0f, 1.0f,      -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 1.0f ,	 0.0f, 1.0f,      -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 1.0f ,	 0.0f, 0.0f,	  -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f ,	 1.0f, 0.0f,	  -1.0f, 0.0f, 0.0f,

		0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 0.0f ,	 1.0f, 0.0f,	  1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f ,	 1.0f, 1.0f,	  1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 0.0f ,	 0.0f, 1.0f,	  1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 0.0f ,	 0.0f, 1.0f,	  1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 0.0f ,	 0.0f, 0.0f,	  1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 0.0f ,	 1.0f, 0.0f,	  1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,	0.5f, 1.0f, 1.0f ,	 0.0f, 1.0f,      0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		0.5f, 1.0f, 1.0f ,	 1.0f, 1.0f,	  0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f,		0.5f, 1.0f, 1.0f ,	 1.0f, 0.0f,	  0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f,		0.5f, 1.0f, 1.0f ,	 1.0f, 0.0f,	  0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,		0.5f, 1.0f, 1.0f ,	 0.0f, 0.0f,	  0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.5f, 1.0f, 1.0f ,	 0.0f, 1.0f,      0.0f, -1.0f, 0.0f,

		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 1.0f ,	 0.0f, 1.0f,	  0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 1.0f ,	 1.0f, 1.0f,	  0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 1.0f ,	 1.0f, 0.0f,	  0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 1.0f ,	 1.0f, 0.0f,	  0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 1.0f ,	 0.0f, 0.0f,	  0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 1.0f ,	 0.0f, 1.0f,	  0.0f, 1.0f, 0.0f
	};
		
	//float vertices[] = {
	//	1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
	//	1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	//	-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
	//	-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	//};										 
	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 11, (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 11, (void*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 11, (void*)(sizeof(float) * 6));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 11, (void*)(sizeof(float) * 8));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	//--------------light-----------//
	glGenVertexArrays(1, &lampVAO);
	glBindVertexArray(lampVAO);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT , GL_FALSE , 11 * sizeof(float) , (void*)0);
	glEnableVertexAttribArray(0);

}
GLFWwindow* initGL(){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(winW, winH, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}

	glViewport(0, 0, winW, winH);

	return window;
}

int main() {
	
	GLFWwindow* window = initGL();
	
	unsigned int textureId_0 = createTexture("container2.png",true);
	unsigned int textureId_1 = createTexture("container2_specular.png", true);

	initVAO();

	Shader lampShader("shader/lampShader.vsh", "shader/lampShader.hlsl");
	lampShader.use();

	Shader lightShader("shader/lightShader.vsh", "shader/lightShader.hlsl");
	lightShader.use();
	lightShader.setInt("material.diffuse", 0);
	lightShader.setInt("material.specular", 1);

	glEnable(GL_DEPTH_TEST);



	std::function<void(Shader, glm::vec3, glm::vec3, float)> createCube = [=](Shader shader, glm::vec3 pos, glm::vec3 scale, float angle) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId_0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureId_1);
		
		glm::mat4 model;
		model = glm::translate(model, pos);
		model = glm::scale(model, scale);
		model = glm::rotate(model, angle, glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setMat4("model", model);

		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("view", view);

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Zoom), winW / winH, 0.1f, 100.0f);
		shader.setMat4("projection", projection);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	};
	
	glm::vec3 cubePositions[] = {
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

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//------------------light-------------//
		glUseProgram(lightShader.ID);
		glBindVertexArray(lightVAO);
		lightShader.setFloat("material.shininess", 32.0f);

		lightShader.setVec3("spotLight.ambient", 0.1f, 0.1f, 0.1f);
		lightShader.setVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
		lightShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);

		lightShader.setVec3("spotLight.position", camera.Position);
		lightShader.setVec3("spotLight.direction", camera.Front);
		lightShader.setFloat("spotLight.cutoff", glm::cos(glm::radians(12.5f)));
		lightShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));

		lightShader.setFloat("spotLight.constant", 1.0f);
		lightShader.setFloat("spotLight.linear", 0.09f);
		lightShader.setFloat("spotLight.quadratic", 0.032f);


		lightShader.setVec3( "pointLights[0].position" , pointLightPositions[0]);
		lightShader.setVec3( "pointLights[0].ambient", 0.1f, 0.1f, 0.1f);
		lightShader.setVec3( "pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		lightShader.setVec3( "pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		lightShader.setFloat("pointLights[0].constant", 1.0f);
		lightShader.setFloat("pointLights[0].linear", 0.09f);
		lightShader.setFloat("pointLights[0].quadratic", 0.032f);

		lightShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		lightShader.setVec3("pointLights[1].ambient", 0.1f, 0.1f, 0.1f);
		lightShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		lightShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		lightShader.setFloat("pointLights[1].constant", 1.0f);
		lightShader.setFloat("pointLights[1].linear", 0.09f);
		lightShader.setFloat("pointLights[1].quadratic", 0.032f);

		lightShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		lightShader.setVec3("pointLights[2].ambient", 0.1f, 0.1f, 0.1f);
		lightShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		lightShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		lightShader.setFloat("pointLights[2].constant", 1.0f);
		lightShader.setFloat("pointLights[2].linear", 0.09f);
		lightShader.setFloat("pointLights[2].quadratic", 0.032f);

		lightShader.setVec3("pointLights[3].position", pointLightPositions[3]);
		lightShader.setVec3("pointLights[3].ambient", 0.1f, 0.1f, 0.1f);
		lightShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		lightShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		lightShader.setFloat("pointLights[3].constant", 1.0f);
		lightShader.setFloat("pointLights[3].linear", 0.09f);
		lightShader.setFloat("pointLights[3].quadratic", 0.032f);

		lightShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		lightShader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
		lightShader.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
		lightShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);


		lightShader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);

		for (int i = 0; i < sizeof(cubePositions) / sizeof(glm::vec3); i++) {
			createCube(lightShader, cubePositions[i], glm::vec3(1), glm::radians(i * 20.0f));
		}

		for (int i = 0; i < sizeof(pointLightPositions) / sizeof(pointLightPositions[0]); i++) {
			glUseProgram(lampShader.ID);
			glBindVertexArray(lampVAO);
			createCube(lampShader, pointLightPositions[i], glm::vec3(0.2), 0);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &lightVAO);
	glDeleteVertexArrays(1, &lampVAO);
	glDeleteBuffers(1, &_VBO);
	glDeleteBuffers(1, &_EBO);

	glfwTerminate();
	return 0;
}

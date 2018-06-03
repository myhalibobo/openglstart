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
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f ,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f , 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f , 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f , 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f ,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f ,  0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f ,  0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f , 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f , 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f , 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f ,  0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f ,  0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f ,  1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f ,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f ,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f ,  0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f ,  0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f ,  1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f , 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f , 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f , 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f , 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f , 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f , 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f ,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f , 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f , 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f , 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f ,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f ,  0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f ,  0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f , 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f , 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f , 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f ,  0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f ,  0.0f, 1.0f
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//--------------light-----------//
	glGenVertexArrays(1, &lampVAO);
	glBindVertexArray(lampVAO);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT , GL_FALSE , 8 * sizeof(float) , (void*)0);
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
	//glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
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
	
	unsigned int textureId_0 = createTexture("wall.jpg",false);
	unsigned int textureId_1 = createTexture("awesomeface.png", true);

	initVAO();
	Shader lampShader("shader/lightShader.vsh", "shader/lampShader.hlsl");
	lampShader.use();
	lampShader.setInt("ourTexture0", 0);
	lampShader.setInt("ourTexture1", 1);

	Shader lightShader("shader/lightShader.vsh", "shader/lightShader.hlsl");
	lightShader.use();
	lightShader.setInt("ourTexture0", 0);
	lightShader.setInt("ourTexture1", 1);
	lightShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	lightShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);

	std::function<void(unsigned int, glm::vec3, glm::vec3)> createCube = [=](unsigned int ID, glm::vec3 pos, glm::vec3 scale) {
		glUseProgram(ID);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId_0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureId_1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glBindVertexArray(lightVAO);

		glm::mat4 model;
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, scale);
		model = glm::translate(model, pos);

		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Zoom), winW / winH, 0.1f, 100.0f);
		glm::mat4 transform;
		transform = projection * view * model;

		unsigned int trans = glGetUniformLocation(ID, "trans");
		glUniformMatrix4fv(trans, 1, GL_FALSE, glm::value_ptr(transform));

		glDrawArrays(GL_TRIANGLES, 0, 36);
	};
	

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		createCube(lightShader.ID, glm::vec3(0), glm::vec3(1));
		
		glBindVertexArray(lampVAO);
		createCube(lampShader.ID, lightPos, glm::vec3(0.2));


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &_VBO);
	glDeleteBuffers(1, &_EBO);

	glfwTerminate();
	return 0;
}

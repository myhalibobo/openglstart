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

#include "model.h"
const float winW = 800;
const float winH = 600;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool firstMouse = true;
float lastX = winW / 2.0f;
float lastY = winH / 2.0f;
unsigned int _VBO, lightVAO, _EBO, lampVAO, floorVAO, floorVBO;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::vec3 lightPos(1.0f, 0.0f, 1.0f);

glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

float planeVertices[] = {
	5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

	5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
	5.0f, -0.5f, -5.0f,  2.0f, 2.0f
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
		-0.5f, -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,  0.0f, -1.0f,  
         0.5f, -0.5f, -0.5f, 1.0f,  0.0f, 0.0f,  0.0f, -1.0f,  
         0.5f,  0.5f, -0.5f, 1.0f,  1.0f, 0.0f,  0.0f, -1.0f,  
         0.5f,  0.5f, -0.5f, 1.0f,  1.0f, 0.0f,  0.0f, -1.0f,  
        -0.5f,  0.5f, -0.5f, 0.0f,  1.0f, 0.0f,  0.0f, -1.0f,  
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,  0.0f, -1.0f,  

        -0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 0.0f,  0.0f,  1.0f,  
         0.5f, -0.5f,  0.5f, 1.0f,  0.0f, 0.0f,  0.0f,  1.0f,  
         0.5f,  0.5f,  0.5f, 1.0f,  1.0f, 0.0f,  0.0f,  1.0f,  
         0.5f,  0.5f,  0.5f, 1.0f,  1.0f, 0.0f,  0.0f,  1.0f,  
        -0.5f,  0.5f,  0.5f, 0.0f,  1.0f, 0.0f,  0.0f,  1.0f,  
        -0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 0.0f,  0.0f,  1.0f,  

        -0.5f,  0.5f,  0.5f, 1.0f,  0.0f,-1.0f,  0.0f,  0.0f,  
        -0.5f,  0.5f, -0.5f, 1.0f,  1.0f,-1.0f,  0.0f,  0.0f,  
        -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,-1.0f,  0.0f,  0.0f,  
        -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,-1.0f,  0.0f,  0.0f,  
        -0.5f, -0.5f,  0.5f, 0.0f,  0.0f,-1.0f,  0.0f,  0.0f,  
        -0.5f,  0.5f,  0.5f, 1.0f,  0.0f,-1.0f,  0.0f,  0.0f,  

         0.5f,  0.5f,  0.5f, 1.0f,  0.0f, 1.0f,  0.0f,  0.0f,  
         0.5f,  0.5f, -0.5f, 1.0f,  1.0f, 1.0f,  0.0f,  0.0f,  
         0.5f, -0.5f, -0.5f, 0.0f,  1.0f, 1.0f,  0.0f,  0.0f,  
         0.5f, -0.5f, -0.5f, 0.0f,  1.0f, 1.0f,  0.0f,  0.0f,  
         0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,  0.0f,  0.0f,  
         0.5f,  0.5f,  0.5f, 1.0f,  0.0f, 1.0f,  0.0f,  0.0f,  

        -0.5f, -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, -1.0f,  0.0f,  
         0.5f, -0.5f, -0.5f, 1.0f,  1.0f, 0.0f, -1.0f,  0.0f,  
         0.5f, -0.5f,  0.5f, 1.0f,  0.0f, 0.0f, -1.0f,  0.0f,  
         0.5f, -0.5f,  0.5f, 1.0f,  0.0f, 0.0f, -1.0f,  0.0f,  
        -0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, -1.0f,  0.0f,  
        -0.5f, -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, -1.0f,  0.0f,  

        -0.5f,  0.5f, -0.5f, 0.0f,  1.0f, 0.0f,  1.0f,  0.0f,  
         0.5f,  0.5f, -0.5f, 1.0f,  1.0f, 0.0f,  1.0f,  0.0f,  
         0.5f,  0.5f,  0.5f, 1.0f,  0.0f, 0.0f,  1.0f,  0.0f,  
         0.5f,  0.5f,  0.5f, 1.0f,  0.0f, 0.0f,  1.0f,  0.0f,  
        -0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 0.0f,  1.0f,  0.0f,  
        -0.5f,  0.5f, -0.5f, 0.0f,  1.0f, 0.0f,  1.0f,  0.0f,  
	};
		
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


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

	//-----------------floor--------------------//
	glGenVertexArrays(1, &floorVAO);
	glBindVertexArray(floorVAO);

	glGenBuffers(1, &floorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
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
	//unsigned int textureId_1 = createTexture("container2_specular.png", true);

	initVAO();

	Shader deepShader("shader/heighGL.vsh", "shader/heighGL.hlsl");
	deepShader.use();

	Shader profileShader("shader/heighGL.vsh", "shader/profile.hlsl");
	profileShader.use();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 0xff);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);


	std::function<void(Shader, glm::vec3, glm::vec3, float)> createCube = [=](Shader shader, glm::vec3 pos, glm::vec3 scale, float angle) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId_0);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, textureId_1);
		
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

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glm::mat4 model;
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)winW / (float)winH, 0.1f, 100.0f);

		profileShader.use();
		profileShader.setMat4("view", view);
		profileShader.setMat4("projection", projection);

		deepShader.use();
		deepShader.setMat4("view", view);
		deepShader.setMat4("projection", projection);

		glStencilMask(0x00);
		// floor
		glBindVertexArray(floorVAO);
		glBindTexture(GL_TEXTURE_2D, textureId_0);
		deepShader.setMat4("model", glm::mat4());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//---------------------------------//

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);

		glBindVertexArray(lightVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId_0);

		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		deepShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		deepShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		//2 second
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		float scale = 1.1f;
		profileShader.use();
		glBindVertexArray(lightVAO);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(scale));
		profileShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scale));
		profileShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);

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

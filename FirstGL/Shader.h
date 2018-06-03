#pragma once
#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>
#include "glm/gtc/matrix_transform.hpp"
#include "string"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

class Shader {
public:
	unsigned int ID;
	Shader(const GLchar * vertexPath, const GLchar *fragmentPath);
	void initShader(const char *vsh, const char *fsh);
	void use();
	void setBool(const string &name, bool value) const;
	void setInt(const string &name, int value) const;
	void setFloat(const string &name, float value) const;
	void setVec3(const string &name, float x, float y, float z) const;

	void setMat4(const string &name, glm::mat4 &mat) const {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
};

#endif
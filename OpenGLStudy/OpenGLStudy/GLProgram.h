//
//  GLProgram.h
//  openglStudy
//
//  Created by apple on 2019/9/20.
//

#ifndef GLProgram_h
#define GLProgram_h

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <functional>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class GLProgram
{
public:
    unsigned int program_id;
    unsigned int vertex_shader;
    unsigned int fragment_shader;
public:
    GLProgram(const char* vertex_source, const char* fragment_source);
    ~GLProgram();
    
    void use() {
        glUseProgram(program_id);
    }
    
    std::string read_file(const char* path) {
        std::ifstream stream;
        stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            stream.open(path);
            std::stringstream strStream;
            strStream << stream.rdbuf();
            stream.close();
            std::string st = strStream.str();
            return strStream.str();
        }
        catch(std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        return "";
    }
    
    void setBool(const std::string &name, bool value) {
        glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
    }
    void setFloat(const std::string &name, float value) {
        glUniform1f(glGetUniformLocation(program_id,name.c_str()), value);
    }
    void setInt(const std::string &name, float value) {
        glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
    }
    void setFloat4(const std::string &name, float p1, float p2, float p3, float p4) {
        glUniform4f(glGetUniformLocation(program_id, name.c_str()), p1, p2, p3, p4);
    }
    void setMat4(const std::string &name, glm::mat4 &mat) {
        glUniformMatrix4fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }
    
private:
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};

GLProgram::GLProgram(const char* vertex_path , const char* fragment_path)
{
    program_id = glCreateProgram();
    // vertex shader
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    
    std::string str = read_file(vertex_path);
    const char * vertex_source = str.c_str();
    glShaderSource(vertex_shader, 1, &vertex_source, NULL);
    glCompileShader(vertex_shader);
    checkCompileErrors(vertex_shader, "VERTEX");
    
    // fragment shader
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    str = read_file(fragment_path);
    const char * fragment_source = str.c_str();
    glShaderSource(fragment_shader, 1, &fragment_source, NULL);
    glCompileShader(fragment_shader);
    checkCompileErrors(vertex_shader, "FRAGMANT");
    
    //program
    program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader);
    glAttachShader(program_id, fragment_shader);
    glLinkProgram(program_id);
    checkCompileErrors(program_id, "PROGRAM");
    
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}


GLProgram::~GLProgram()
{
    
}

#endif /* GLProgram_h */

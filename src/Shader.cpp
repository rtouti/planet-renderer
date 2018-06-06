#include "Shader.h"

#include <GL/gl3w.h>
#include <iostream>

Shader::Shader(const char* vertexSrc, const char* fragmentSrc){
	this->vertexSrc = vertexSrc;
	this->fragmentSrc = fragmentSrc;

	vertexID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexID, 1, &vertexSrc, NULL);
	glCompileShader(vertexID);
	GLchar vertexLog[512];
	glGetShaderInfoLog(vertexID, 512, NULL, vertexLog);
	std::cout << vertexLog << std::endl;

	fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentID, 1, &fragmentSrc, NULL);
	glCompileShader(fragmentID);
	GLchar fragmentLog[512];
	glGetShaderInfoLog(fragmentID, 512, NULL, fragmentLog);
	std::cout << fragmentLog << std::endl;

	programID = glCreateProgram();
	glAttachShader(programID, vertexID);
	glAttachShader(programID, fragmentID);
	glLinkProgram(programID);
	glDetachShader(programID, vertexID);
	glDetachShader(programID, fragmentID);
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);


	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
}

Shader::~Shader(){
	glDeleteProgram(programID);
}

void Shader::Use(){
	glUseProgram(programID);
}

int Shader::GetAttribLocation(const char* name){
	glGetAttribLocation(programID, name);
}

int Shader::GetUniformLocation(const char* name){
	glGetUniformLocation(programID, name);
}

void Shader::SetUniform1f(const char* name, float a){
	glUniform1f(glGetUniformLocation(programID, name), a);
}

void Shader::SetUniform1i(const char* name, int a){
	glUniform1i(glGetUniformLocation(programID, name), a);
}

void Shader::SetUniform1d(const char* name, double a){
	glUniform1d(glGetUniformLocation(programID, name), a);
}

void Shader::SetUniform3d(const char* name, double a, double b, double c){
	glUniform3d(glGetUniformLocation(programID, name), a, b, c);
}

void Shader::SetUniformMat4fv(const char* name, const float* value){
	glUniformMatrix4fv(glGetUniformLocation(programID, name), 1, GL_FALSE, value);
}

void Shader::SetUniformMat4dv(const char* name, const double* value){
	glUniformMatrix4dv(glGetUniformLocation(programID, name), 1, GL_FALSE, value);
}

void Shader::Add(std::string name, const char* vertexSrc, const char* fragmentSrc){
    Shader::Shaders[name] = new Shader(vertexSrc, fragmentSrc);
}

Shader* Shader::Get(std::string name){
    if(Shader::Shaders.find(name) == Shader::Shaders.end())
        return nullptr;

    return Shader::Shaders[name];
}

std::map<std::string, Shader*> Shader::Shaders;

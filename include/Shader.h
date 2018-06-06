#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include <string>
#include <map>


class Shader {

public:
    ~Shader();
    void Use();
    int GetAttribLocation(const char* name);
    int GetUniformLocation(const char* name);
    void SetUniform1f(const char* name, float a);
    void SetUniform1i(const char* name, int a);
    void SetUniform1d(const char* name, double a);
    void SetUniform3d(const char* name, double a, double b, double c);
    void SetUniformMat4fv(const char* name, const float* value);
    void SetUniformMat4dv(const char* name, const double* value);
private:
    Shader(const char* vertexSrc, const char* fragmentSrc);
    const char *vertexSrc, *fragmentSrc;
    int vertexID, fragmentID, programID;

public:
    static void Add(std::string name, const char* vertexSrc, const char* fragmentSrc);
    static Shader* Get(std::string name);
private:
    static std::map<std::string, Shader*> Shaders;

};

#endif // SHADER_H_INCLUDED

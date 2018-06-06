#include <GL/gl3w.h>
#include "Terrain.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Terrain::Terrain(double size, int segments, double x, double y, double z)
: size(size), segments(segments), x(x), y(y), z(z)
{
    if(Shader::Get("terrain") == nullptr)
        Shader::Add("terrain", Terrain::VertexSrc, Terrain::FragmentSrc);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    BuildMesh();
}

Terrain::~Terrain()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void Terrain::BuildMesh(){
    double segmentSize = size/segments;
    double halfSize = size*0.5;
    for(double zz = -halfSize; zz <= halfSize; zz+=segmentSize){
        for(double xx = -halfSize; xx <= halfSize; xx+=segmentSize){
            vertices.push_back(xx);
            vertices.push_back(y);
            vertices.push_back(zz);
        }
    }
    unsigned int index = 0;
    for(double zz = -halfSize; zz < halfSize; zz+=segmentSize){
        for(double xx = -halfSize; xx < halfSize; xx+=segmentSize){
            indices.push_back(index);
            indices.push_back(index+segments+1);
            indices.push_back(index+segments+2);
            indices.push_back(index);
            indices.push_back(index+segments+2);
            indices.push_back(index+1);
            index++;
        }
        index++;
    }

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(double), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribLPointer(0, 3, GL_DOUBLE, 3*sizeof(double), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Terrain::Render(Camera& camera){
    glm::dmat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    Shader* shader = Shader::Get("terrain");
    shader->Use();
    shader->SetUniformMat4dv("uProj", glm::value_ptr(camera.GetProjectionMatrix()));
    shader->SetUniformMat4dv("uView", glm::value_ptr(camera.GetViewMatrix()));
    shader->SetUniformMat4dv("uModel", glm::value_ptr(model));
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


char* Terrain::VertexSrc =
"#version 440 core\n"
"\n"
"layout(location=0) in dvec3 aPos;"
"\n"
"uniform dmat4 uProj;\n"
"uniform dmat4 uView;\n"
"uniform dmat4 uModel;\n"
"\n"
"void main(){\n"
"   //gl_Position = vec4(dvec4(aPos, 1.0));\n"
"   gl_Position = vec4(uProj*uView*uModel*dvec4(aPos, 1.0));\n"
"   float w = gl_Position.w;\n"
"   gl_Position.z = (2.0*log(1.0*w+1.0) / log(1.0*100000000000000.0+1.0) - 1.0)*w;\n"
"}";

char* Terrain::FragmentSrc =
"#version 440 core\n"
"\n"
"out vec4 vColor;"
"\n"
"void main(){\n"
"       vColor = vec4(0.0f, 0.0f, 0.9f, 1.0f);\n"
"}";

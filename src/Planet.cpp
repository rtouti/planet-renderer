#include <GL/gl3w.h>
#include "Planet.h"

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <algorithm>




Planet::Planet(double radius)
: radius(radius)
{
    if(Shader::Get("planet") == nullptr)
        Shader::Add("planet", Planet::VertexSrc, Planet::FragmentSrc);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    //CreateBaseMesh();
}

Planet::~Planet()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void Planet::BuildMesh(){


    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(int), indices.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Planet::Render(Camera& camera){
    glm::dmat4 model = glm::translate(glm::dmat4(1.0f), glm::dvec3(0.0f, 0.0f, 0.0f));

    Shader* shader = Shader::Get("planet");
    shader->Use();
    shader->SetUniformMat4dv("uProj", glm::value_ptr(camera.GetProjectionMatrix()));
    shader->SetUniformMat4dv("uView", glm::value_ptr(camera.GetViewMatrix()));
    shader->SetUniformMat4dv("uModel", glm::value_ptr(model));
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


char* Planet::VertexSrc =
"#version 330 core\n"
"\n"
"layout(location=0) in vec3 aPos;"
"\n"
"uniform mat4 uProj;\n"
"uniform mat4 uView;\n"
"uniform mat4 uModel;\n"
"\n"
"void main(){\n"
"   gl_Position = uProj*uView*uModel*vec4(aPos, 1.0f);\n"
"   float w = gl_Position.w;\n"
"   gl_Position.z = (2.0*log(1.0*w+1.0) / log(1.0*100000000000000.0+1.0) - 1.0)*w;\n"
"}";

char* Planet::FragmentSrc =
"#version 330 core\n"
"\n"
"out vec4 vColor;"
"\n"
"void main(){\n"
"   vColor = vec4(0.f, 0.0f, 0.9f, 1.0f);\n"
"}";

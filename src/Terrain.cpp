#include <GL/gl3w.h>
#include "Terrain.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Terrain::Terrain(double x, double y, double z, double size, int segments)
: x(x), y(y), z(z), size(size), segments(segments)
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
    double segmentSize = 1.0/segments;
    double halfSize = 0.5;
    for(double zz = 0.0; zz <= 1.0; zz+=segmentSize){
        for(double xx = 0.0; xx <= 1.0; xx+=segmentSize){
            vertices.push_back(xx);
            vertices.push_back(0.0);
            vertices.push_back(zz);
        }
    }
    unsigned int index = 0;
    for(double zz = 0.0; zz < 1.0-0.001; zz+=segmentSize){
        for(double xx = 0.0; xx < 1.0-0.001; xx+=segmentSize){
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
    shader->SetUniform1d("uSize", size);
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    _Render(camera, Rect(0.0, 0.0, 1.0), shader, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Terrain::_Render(Camera& camera, const Rect& rect, Shader* shader, int depth){
    glm::dvec3 cameraPosition = camera.GetPosition();

    double halfRectSize = rect.size*0.5;
    double halfSize = size*0.5;
    glm::dvec3 center = glm::dvec3((rect.x+halfRectSize)*size - halfSize, 0.0, (rect.y+halfRectSize)*size - halfSize);
    glm::dvec3 corner = glm::dvec3((rect.x+rect.size)*size - halfSize, 0.0, (rect.y+rect.size)*size - halfSize);
    double centerToCorner = glm::distance(center, corner);
    double centerToCamera = glm::distance(center, cameraPosition);

    if(depth <= 7 && centerToCamera < centerToCorner*2.0){
        for(int q = 0; q < 4; q++){
            _Render(camera, rect.GetQuadrant(q), shader, depth+1);
        }
    }
    else {
        shader->SetUniform3d("uRect", rect.x, rect.y, rect.size);
        //std::cout << rect.x << " " << rect.y << " " << rect.size << std::endl;
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
}


char* Terrain::VertexSrc =
"#version 440 core\n"
"\n"
"layout(location=0) in dvec3 aPos;"
"\n"
"uniform dmat4 uProj;\n"
"uniform dmat4 uView;\n"
"uniform dmat4 uModel;\n"
"uniform dvec3 uRect;\n"
"uniform double uSize;\n"
"\n"
"void main(){\n"
"   dvec3 position = dvec3(uRect.x+aPos.x*uRect.z, 0.0, uRect.y+aPos.z*uRect.z);\n"
"   position.xz *= uSize;\n"
"   position.xz -= uSize*0.5;\n"
"   gl_Position = vec4(uProj*uView*uModel*dvec4(position, 1.0));\n"
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

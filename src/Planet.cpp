#include <GL/gl3w.h>
#include "Planet.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include "Texture.h"
#include "TextureGenerator.h"

Planet::Planet(double x, double y, double z, double size, int segments)
: x(x), y(y), z(z), size(size), segments(segments)
{
    if(Shader::Get("planet") == nullptr)
        Shader::Add("planet", Planet::VertexSrc, Planet::FragmentSrc);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    BuildMesh();
}

Planet::~Planet()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void Planet::BuildMesh(){
    double segmentSize = 1.0/segments;
    double halfSize = 0.5;
    for(int zz = 0; zz <= segments; zz++){
        for(int xx = 0; xx <= segments; xx++){
            vertices.push_back(xx*segmentSize);
            vertices.push_back(0.0);
            vertices.push_back(zz*segmentSize);
        }
    }
    unsigned int index = 0;
    //TODO: Replace double zz and double xx by int zz and int xx (for int zz = 0; zz < segments; z++)...
    //DONE
    for(int zz = 0; zz < segments; zz++){
        for(int xx = 0; xx < segments; xx++){
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

void Planet::Render(Camera& camera){
    glm::dmat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    Shader* shader = Shader::Get("planet");
    //shader->Use();
    //shader->SetUniformMat4dv("uProj", glm::value_ptr(camera.GetProjectionMatrix()));
    //shader->SetUniformMat4dv("uView", glm::value_ptr(camera.GetViewMatrix()));
    //shader->SetUniformMat4dv("uModel", glm::value_ptr(model));
    //shader->SetUniform1d("uSize", size);
    //glBindVertexArray(vao);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    for(int f = 0; f < 6; f++){
        //shader->SetUniformMat4dv("uFace", glm::value_ptr(Planet::FaceMatrices[f]));
        _Render(camera, f, Rect(0.0, 0.0, 1.0), shader, 0);
    }

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);
}

void Planet::_Render(Camera& camera, int face, const Rect& rect, Shader* shader, int depth){
    glm::dvec3 cameraPosition = camera.GetPosition();
    double halfRectSize = rect.size*0.5;
    double halfSize = size*0.5;

    //Frustum culling
    /*glm::dmat4 view = camera.GetViewMatrix();
    glm::dmat4 projection = camera.GetProjectionMatrix();
    glm::dvec4 corners[4] = {
        halfSize*glm::normalize(glm::dvec4((rect.x)*size - halfSize, halfSize, (rect.y)*size - halfSize, 1.0)),
        halfSize*glm::normalize(glm::dvec4((rect.x+rect.size)*size - halfSize, halfSize, (rect.y)*size - halfSize, 1.0)),
        halfSize*glm::normalize(glm::dvec4((rect.x+rect.size)*size - halfSize, halfSize, (rect.y+rect.size)*size - halfSize, 1.0)),
        halfSize*glm::normalize(glm::dvec4((rect.x)*size - halfSize, halfSize, (rect.y+rect.size)*size - halfSize, 1.0))
    };
    for(int c = 0; c < 4; c++){
        glm::dvec4 p = projection*view*Planet::FaceMatrices[face]*corners[c];
        p /= p.w;
        if(p.x >= -1.0 && p.x <= 1.0 && p.y >= -1.0 && p.y <= 1.0)
            break;

        //If all four corners are not visible, don't draw the chunk
        return;
    }*/

    glm::dvec4 center = halfSize*glm::normalize(glm::dvec4((rect.x+halfRectSize)*size - halfSize, halfSize, (rect.y+halfRectSize)*size - halfSize, 1.0));
    glm::dvec4 corner = halfSize*glm::normalize(glm::dvec4((rect.x+rect.size)*size - halfSize, halfSize, (rect.y+rect.size)*size - halfSize, 1.0));
    center = Planet::FaceMatrices[face]*center;
    corner = Planet::FaceMatrices[face]*corner;
    double centerToCorner = glm::distance(center, corner);
    double centerToCamera = glm::distance(center, glm::dvec4(cameraPosition, 1.0));

    //if(depth == 15)
    //    std::cout << depth << std::endl;
    if(depth <= 12 && centerToCamera < centerToCorner*2.0){
        for(int q = 0; q < 4; q++){
            _Render(camera, face, rect.GetQuadrant(q), shader, depth+1);
        }
    }
    else {
        //Create a unique id for each texture
        //std::string id = std::to_string(rect.x) + " " + std::to_string(rect.y) + " " + std::to_string(rect.size) + " " + std::to_string(face);
        std::stringstream ss;
        ss << rect.x << " " << rect.y << " " << rect.size << " " << face;
        std::string id = ss.str();
        if(Texture::Get(id) == nullptr)
            TextureGenerator::GenerateTerrainTexture(id, rect, face);

        shader->Use();
        shader->SetUniformMat4dv("uProj", glm::value_ptr(camera.GetProjectionMatrix()));
        shader->SetUniformMat4dv("uView", glm::value_ptr(camera.GetViewMatrix()));
        shader->SetUniformMat4dv("uModel", glm::value_ptr(model));
        shader->SetUniform1d("uSize", size);
        shader->SetUniform3d("uRect", rect.x, rect.y, rect.size);
        shader->SetUniformMat4dv("uFace", glm::value_ptr(Planet::FaceMatrices[face]));
        shader->SetUniform1i("uUnit", 0);

        Texture* texture = Texture::Get(id);
        texture->Bind();

        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        texture->UnBind();
    }
}


glm::dmat4 Planet::FaceMatrices[6] = {
    glm::rotate(glm::dmat4(1.0), -3.14159*0.5, glm::dvec3(1.0, 0.0, 0.0)),           //Front face
    glm::rotate(glm::dmat4(1.0), 3.14159*0.5, glm::dvec3(1.0, 0.0, 0.0)),       //Back face
    glm::rotate(glm::dmat4(1.0), 3.14159*0.5, glm::dvec3(0.0, 0.0, 1.0)),           //Right face
    glm::rotate(glm::dmat4(1.0), -3.14159*0.5, glm::dvec3(0.0, 0.0, 1.0)),           //Left face
    glm::rotate(glm::dmat4(1.0), 0.0, glm::dvec3(1.0, 0.0, 0.0)),           //Top face
    glm::rotate(glm::dmat4(1.0), 3.14159, glm::dvec3(1.0, 0.0, 0.0))            //Bottom face
};


char* Planet::VertexSrc =
"#version 440 core\n"
"\n"
"layout(location=0) in dvec3 aPos;"
"\n"
"out double vHeight;\n"
"out vec2 vRect;\n"
"\n"
"uniform dmat4 uProj;\n"
"uniform dmat4 uView;\n"
"uniform dmat4 uModel;\n"
"uniform dvec3 uRect;\n"
"uniform double uSize;\n"
"uniform dmat4 uFace;\n"
"uniform sampler2D uUnit;\n"
"\n"
"void main(){\n"
"   float h = texture(uUnit, vec2(aPos.xz)).a;\n"
"   h *= 2.0f;\n"
"   h -= 1.0f;\n"
"   h *= 3000000.0f;\n"
"   dvec3 position = dvec3(uRect.x+aPos.x*uRect.z, uSize*0.5, uRect.y+aPos.z*uRect.z);\n"
"   position.xz *= uSize;\n"
"   position.xz -= uSize*0.5;\n"
"   position = (0.5*uSize+h)*normalize(vec3(uFace*vec4(position, 1.0)));\n"
"   gl_Position = vec4(uProj*uView*uModel*dvec4(position, 1.0));\n"
"\n"
"   //Logarithmic depth buffer\n"
"   float w = gl_Position.w;\n"
"   gl_Position.z = (2.0*log(1.0*w+1.0) / log(1.0*100000000000000.0+1.0) - 1.0)*w;\n"
"   vRect = vec2(aPos.x, aPos.z);\n"
"}";

char* Planet::FragmentSrc =
"#version 440 core\n"
"\n"
"in vec2 vRect;\n"
"\n"
"out vec4 vColor;"
"\n"
"uniform sampler2D uUnit;\n"
"\n"
"void main(){\n"
"   vColor = texture(uUnit, vRect);\n"
"}";

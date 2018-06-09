#include "GL/gl3w.h"
#include "TextureGenerator.h"
#include "Shader.h"
#include "Planet.h"
#include "Texture.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

TextureGenerator::TextureGenerator(){}

TextureGenerator::~TextureGenerator(){}

void TextureGenerator::GenerateTerrainTexture(std::string name, const Rect& rect, int face){
    if(Shader::Get("terrain generator") == nullptr)
        Shader::Add("terrain generator", TextureGenerator::VertexSrc, TextureGenerator::FragmentSrc);

    float data[24] = {
        -1.0, -1.0, rect.x, rect.y,
        1.0, -1.0, rect.x+rect.size, rect.y,
        1.0, 1.0, rect.x+rect.size, rect.y+rect.size,
        1.0, 1.0, rect.x+rect.size, rect.y+rect.size,
        -1.0, 1.0, rect.x, rect.y+rect.size,
        -1.0, -1.0, rect.x, rect.y
    };
    Shader* shader = Shader::Get("terrain generator");
    unsigned int vao, vbo, fbo, texID;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenFramebuffers(1, &fbo);
    glGenTextures(1, &texID);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    glViewport(0, 0, 256, 256);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    //glGenerateMipmap(gl.TEXTURE_2D);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texID, 0);

    shader->Use();

    shader->SetUniformMat4dv("uFaceMat", glm::value_ptr(Planet::FaceMatrices[face]));
    shader->SetUniform3d("uRect", rect.x, rect.y, rect.size);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    //std::cout << "texture done" << std::endl;


    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDeleteFramebuffers(1, &fbo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    glViewport(0, 0, 800, 600);

    //let id = quadTreeFace.face+" "+rect.x+" "+rect.y+" "+rect.size;
    //App.app.loader.AddFromWebGLTexture("terrain"+id, texture);

    Texture::Add(name, new Texture(texID));
}


char* TextureGenerator::VertexSrc =
"#version 440 core\n"
"\n"
"layout (location=0) in vec2 aPos;\n"
"layout (location=1) in vec2 aRect;\n"
"\n"
"out vec2 vPos;\n"
"out vec2 vRect;\n"
"\n"
"void main(){\n"
"    gl_Position = vec4(aPos, 0.0, 1.0);\n"
"\n"
"    vPos = aPos;\n"
"    vRect = aRect;\n"
"}\n";

char* TextureGenerator::FragmentSrc =
"#version 440 core\n"
"\n"
"in vec2 vPos;\n"
"in vec2 vRect;\n"
"\n"
"uniform dvec3 uRect;\n"
"uniform dmat4 uFaceMat;\n"
"\n"
"float rand2d(float x, float y){\n"
"	return fract(sin(dot(vec2(x, y) ,vec2(12.9898, 78.233))) * 43758.5453);\n"
"}\n"
"\n"
"float rand3d(float x, float y, float z){\n"
"	return fract(sin(dot(vec3(x, y, z) ,vec3(12.9898, 78.233, 54.734))) * 43758.5453);\n"
"}\n"
"\n"
"float fade(float t){\n"
"	return t*t*(3.0-2.0*t);\n"
"}\n"
"\n"
"float noise2d(float x, float y){\n"
"	float xf = fade(fract(x));\n"
"	float yf = fade(fract(y));\n"
"	float xi = floor(x);\n"
"	float yi = floor(y);\n"
"\n"
"	return mix(\n"
"			mix(rand2d(xi, yi), rand2d(xi+1.0, yi), xf),\n"
"			mix(rand2d(xi, yi+1.0), rand2d(xi+1.0, yi+1.0), xf),\n"
"			yf\n"
"	);\n"
"}\n"
"\n"
"float noise3d(float x, float y, float z){\n"
"	float xf = fade(fract(x));\n"
"	float yf = fade(fract(y));\n"
"	float zf = fade(fract(z));\n"
"	float xi = floor(x);\n"
"	float yi = floor(y);\n"
"	float zi = floor(z);\n"
"\n"
"	return mix(\n"
"		mix(\n"
"			mix(rand3d(xi, yi, zi), rand3d(xi+1.0, yi, zi), xf),\n"
"			mix(rand3d(xi, yi+1.0, zi), rand3d(xi+1.0, yi+1.0, zi), xf),\n"
"			yf\n"
"		),\n"
"		mix(\n"
"			mix(rand3d(xi, yi, zi+1.0), rand3d(xi+1.0, yi, zi+1.0), xf),\n"
"			mix(rand3d(xi, yi+1.0, zi+1.0), rand3d(xi+1.0, yi+1.0, zi+1.0), xf),\n"
"			yf\n"
"		),\n"
"		zf\n"
"	);\n"
"}\n"
"\n"
"void main(){\n"
"   vec4 position = 10.0f*normalize(mat4(uFaceMat)*vec4(vRect.x*2.0f-1.0f, 1.0f, vRect.y*2.0f-1.0f, 1.0f));\n"
"   float h = 0.0f;\n"
"   float amp = 1.0f;\n"
"   float freq = 0.3f;\n"
"   const int octaves = 20;\n"
"   for(int o = 0; o < octaves; o++){\n"
"       float v = amp*noise3d(freq*position.x, freq*position.y, freq*position.z);\n"
"       v *= 2.0f;\n"
"       v -= amp*1.0f;\n"
"       h += v;\n"
"       amp *= 0.5f;\n"
"       freq *= 3.0f;\n"
"   }\n"
"   h += 1.0f;\n"
"   h *= 0.5f;\n"
"   if(h <= 0.5f)\n"
"       gl_FragColor = vec4(0.0f, 0.0f, h, h);\n"
"   else if(h <= 0.8f)\n"
"       gl_FragColor = vec4(h, h*0.8f, h*0.5f, h);\n"
"   else\n"
"       gl_FragColor = vec4(1.0f*h, 1.0f*h, 1.0f*h, h);\n"
"}\n";

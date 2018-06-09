#include <GL/gl3w.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "Texture.h"

Texture::Texture(const char* path)
: path(path)
{
    int width, height;
    unsigned char* data = stbi_load(path, &width, &height, NULL, 4);

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
}

Texture::Texture(unsigned int id)
: id(id)
{

}

Texture::~Texture()
{

}

void Texture::Bind(){
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::UnBind(){
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Add(std::string name, const char* path){
    Texture::Textures[name] = new Texture(path);
}

void Texture::Add(std::string name, Texture* texture){
    Texture::Textures[name] = texture;
}

Texture* Texture::Get(std::string name){
    if(Texture::Textures.find(name) == Texture::Textures.end())
        return nullptr;

    return Texture::Textures[name];
}

std::map<std::string, Texture*> Texture::Textures;

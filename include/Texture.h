#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <map>

class Texture
{
    public:
        Texture(const char* path);
        Texture(unsigned int id);
        virtual ~Texture();
        void Bind();
        void UnBind();
    private:
        const char* path;
        unsigned int id;

    public:
        static void Add(std::string name, const char* path);
        static void Add(std::string name, Texture* texture);
        static Texture* Get(std::string name);
    private:
        static std::map<std::string, Texture*> Textures;
};

#endif // TEXTURE_H

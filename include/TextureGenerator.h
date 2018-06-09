#ifndef TEXTUREGENERATOR_H
#define TEXTUREGENERATOR_H

#include "Texture.h"
#include "Rect.h"

class TextureGenerator
{
    public:
        static void GenerateTerrainTexture(std::string name, const Rect& rect, int face);
    private:
        TextureGenerator();
        virtual ~TextureGenerator();

        static char* VertexSrc;
        static char* FragmentSrc;
};

#endif // TEXTUREGENERATOR_H

#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "Camera.h"
//#include "Rect.h"

class Terrain
{
    public:
        Terrain(double size, int segments, double x, double y, double z);
        virtual ~Terrain();
        void Render(Camera& camera);
    private:
        double size;
        int segments;
        double x, y, z;
        std::vector<double> vertices;
        std::vector<unsigned int> indices;
        glm::dvec3 position;
        glm::dmat4 model;

        unsigned int vao, vbo, ebo;

        void BuildMesh();
        //void _Render(Rect rect);

        static char* VertexSrc;
        static char* FragmentSrc;
};

#endif // TERRAIN_H

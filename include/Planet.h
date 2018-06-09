#ifndef PLANET_H
#define PLANET_H

#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "Camera.h"
#include "Rect.h"

class Planet
{
    public:
        Planet(double x, double y, double z, double size, int segments);
        virtual ~Planet();
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
        void _Render(Camera& camera, int face, const Rect& rect, Shader* shader, int depth);

    public:
        static glm::dmat4 FaceMatrices[6];
    private:
        static char* VertexSrc;
        static char* FragmentSrc;
};

#endif // PLANET_H

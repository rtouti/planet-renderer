#ifndef PLANET_H
#define PLANET_H

#include <glm/vec3.hpp>
#include <cmath>
#include <vector>

#include "Shader.h"
#include "Camera.h"

class Planet
{
    public:
        Planet(double radius);
        virtual ~Planet();
        void Render(Camera& camera);
    private:
        void BuildMesh();

        glm::dvec3 position;
        double radius;
        std::vector<double> vertices;
        std::vector<int> indices;

        unsigned int vao, vbo, ebo;

        static char* VertexSrc;
        static char* FragmentSrc;
};

#endif // PLANET_H

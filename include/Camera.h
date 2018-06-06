#ifndef CAMERA_H
#define CAMERA_H

#include <glm/mat4x4.hpp>

#include "Shader.h"
#include "Window.h"

class Camera
{
    public:
        Camera();
        virtual ~Camera();
        void Update(Window& window);
        glm::dvec3 GetPosition();
        glm::dmat4 GetViewMatrix();
        glm::dmat4 GetProjectionMatrix();
        void SetProjection(double fov, double ratio, double near, double far);

        glm::dvec3 position;

    private:
        glm::dmat4 projection, view;
        double pitch, yaw, sensitivity, speed;
};

#endif // CAMERA_H

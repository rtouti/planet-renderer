#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>


Camera::Camera()
: position(glm::dvec3(0.0f, 0.0f, 0.0f)), projection(glm::dmat4(1.0)), view(glm::dmat4(1.0)), pitch(0.0), yaw(0.0), sensitivity(0.5), speed(100000000.0)
{

}

Camera::~Camera()
{

}

void Camera::Update(Window& window){
    if(window.IsMouseDown(GLFW_MOUSE_BUTTON_LEFT)){
        yaw += window.GetMouseDX()*sensitivity;
        pitch -= window.GetMouseDY()*sensitivity;
    }

    glm::dvec3 front = glm::normalize(glm::vec3(
        cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        sin(glm::radians(pitch)),
        sin(glm::radians(yaw)) * cos(glm::radians(pitch))
    ));
    glm::dvec3 right = glm::normalize(glm::cross(glm::dvec3(0.0f, 1.0f, 0.0f), front));
    glm::dvec3 up = glm::normalize(glm::cross(front, right));

    if(window.IsKeyDown(GLFW_KEY_W))
        position += front*speed;
    else if(window.IsKeyDown(GLFW_KEY_S))
        position -= front*speed;
    if(window.IsKeyDown(GLFW_KEY_D))
        position -= right*speed;
    else if(window.IsKeyDown(GLFW_KEY_A))
        position += right*speed;

    view = glm::lookAt(position, position+front, up);
}

glm::dvec3 Camera::GetPosition(){
    return position;
}

glm::dmat4 Camera::GetViewMatrix(){
    return view;
}

glm::dmat4 Camera::GetProjectionMatrix(){
    return projection;
}

void Camera::SetProjection(double fov, double ratio, double near, double far){
    projection = glm::perspective(fov, ratio, near, far);
}

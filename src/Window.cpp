#include "Window.h"

#include <cstring>

Window::Window(std::string name, int width, int height)
: name(name), width(width), height(height)
{
    glfwInit();

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

    handle = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    glfwMakeContextCurrent(handle);

    memset(keys, 0, 256*sizeof(int));
    memset(previousKeys, 0, 256*sizeof(int));
    memset(buttons, 0, 256*sizeof(int));
    memset(previousButtons, 0, 256*sizeof(int));
    glfwSetWindowUserPointer(handle, this);
    glfwSetKeyCallback(handle, Window::KeyCallback);
    glfwSetCursorPosCallback(handle, Window::MouseMoveCallback);
    glfwSetMouseButtonCallback(handle, Window::MouseButtonCallback);
    glfwSetWindowSizeCallback(handle, Window::ResizeCallback);
}

Window::~Window()
{
    glfwDestroyWindow(handle);
    glfwTerminate();
}

bool Window::IsOpen(){
    return !glfwWindowShouldClose(handle);
}

void Window::PollEvents(){
    glfwPollEvents();
}

void Window::SwapBuffers(){
    glfwSwapBuffers(handle);
}

void Window::Close(){
    glfwSetWindowShouldClose(handle, GLFW_TRUE);
}

void Window::UpdateInput(){
    memcpy(previousKeys, keys, sizeof(keys));
    memcpy(previousButtons, buttons, sizeof(buttons));
    previousMouseX = mouseX;
    previousMouseY = mouseY;
}

bool Window::IsKeyDown(int key){
    return keys[key];
}

bool Window::IsKeyPressed(int key){
    return keys[key] && !previousKeys[key];
}

void Window::SetKey(int key, bool pressed){
    keys[key] = pressed;
}

void Window::SetMousePos(double x, double y){
    mouseX = x;
    mouseY = y;
}

void Window::SetButton(int button, bool pressed){
    buttons[button] = pressed;
}

double Window::GetMouseX(){
    return mouseX;
}

double Window::GetMouseY(){
    return mouseY;
}

double Window::GetMouseDX(){
    return mouseX-previousMouseX;
}

double Window::GetMouseDY(){
    return mouseY-previousMouseY;
}

bool Window::IsMouseDown(int button){
    return buttons[button];
}

bool Window::IsMousePressed(int button){
    return buttons[button] && !previousButtons[button];
}

void Window::SetSize(int width, int height){
    this->width = width;
    this->height = height;
    glViewport(0, 0, width, height);
}

void Window::KeyCallback(GLFWwindow* handle, int key, int scancode, int action, int mods){
    Window* window = (Window*)glfwGetWindowUserPointer(handle);

    if(action == GLFW_PRESS || action == GLFW_REPEAT)
        window->SetKey(key, true);
    else if(action == GLFW_RELEASE)
        window->SetKey(key, false);
}

void Window::MouseMoveCallback(GLFWwindow* handle, double x, double y){
    Window* window = (Window*)glfwGetWindowUserPointer(handle);
    window->SetMousePos(x, y);
}

void Window::MouseButtonCallback(GLFWwindow* handle, int button, int action, int mods){
    Window* window = (Window*)glfwGetWindowUserPointer(handle);

    if(action == GLFW_PRESS)
        window->SetButton(button, true);
    else if(action == GLFW_RELEASE)
        window->SetButton(button, false);
}

void Window::ResizeCallback(GLFWwindow* handle, int width, int height){
    Window* window = (Window*)glfwGetWindowUserPointer(handle);
    window->SetSize(width, height);
}

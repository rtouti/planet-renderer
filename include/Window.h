#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

#include <string>

class Window
{
    public:
        Window(std::string name, int width, int height);
        virtual ~Window();
        bool IsOpen();
        void PollEvents();
        void SwapBuffers();
        void Close();
        void UpdateInput();
        bool IsKeyDown(int key);
        bool IsKeyPressed(int key);
        void SetKey(int key, bool pressed);
        void SetMousePos(double x, double y);
        void SetButton(int button, bool pressed);
        double GetMouseX();
        double GetMouseY();
        double GetMouseDX();
        double GetMouseDY();
        bool IsMouseDown(int button);
        bool IsMousePressed(int button);
        void SetSize(int width, int height);
    private:
        static void KeyCallback(GLFWwindow* handle, int key, int scancode, int action, int mods);
        static void MouseMoveCallback(GLFWwindow* handle, double x, double y);
        static void MouseButtonCallback(GLFWwindow* handle, int button, int action, int mods);
        static void ResizeCallback(GLFWwindow* handle, int width, int height);

        std::string name;
        int width, height;
        GLFWwindow* handle;
        int keys[256];
        int previousKeys[256];
        double mouseX, mouseY;
        double previousMouseX, previousMouseY;
        int buttons[256];
        int previousButtons[256];
};

#endif // WINDOW_H

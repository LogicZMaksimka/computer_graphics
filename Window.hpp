#pragma once

#include <GLFW/glfw3.h>

#include "Model.hpp"
#include "Camera.hpp"

class Window
{
private:
    GLFWwindow *window;
    int width;
    int height;

    glm::mat4 GetProjectionMatrix(int width, int height);
    glm::mat4 GetViewMatrix();
    glm::mat4 CubeMVPMatrix();
public:
    Window(const char *title, int width, int height);

    GLFWwindow *GetWindow(){
      return window;
    }

    GLuint GetWidth(){
      return width;
    }

    GLuint GetHeight(){
      return height;
    }

    // - Set drawing context to current window
    void MakeContextCurrent(){
      glfwMakeContextCurrent(window);
    }

    // This function binds VAO only once - before drawing all the objects
    // TODO: а стоит ли делать функцию рисующую сразу все объекты с одним VAO ????
    void Draw(const Model &model, const Camera &camera);
};
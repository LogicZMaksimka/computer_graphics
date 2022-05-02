#pragma once

#include <glm/glm.hpp>

#include "Window.hpp"

class Camera
{
private:
    Window window;

    double last_update_time;

    float movement_speed = 1.0f;
    float mouse_speed = 0.01f;
    float fov_speed = 1.0f;

    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 dir = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    GLfloat pitch = 0.0f;
    GLfloat yaw = 0.0f;
    GLfloat roll = 0.0f;

    GLfloat fov = 45.0f;

    void UpdateFromKeyboard(GLfloat deltaTime);
    void UpdateFromMouse(GLfloat deltaTime);

public:
    Camera(const Window &window_,
           float movement_speed_ = 1.0f,
           float mouse_speed_ = 0.01f,
           float fov_speed_ = 1.0f) : window(window_),
                                     movement_speed(movement_speed_),
                                     mouse_speed(mouse_speed_),
                                     fov_speed(fov_speed_)
    {
        last_update_time = glfwGetTime();
        // Turn on unlimited rotation and cursor centering
        glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    };

    void Update();

    glm::mat4 GetViewMatrix();
};

// class Camera {

// 	glm::vec3 cameraPos, cameraDir, cameraUp, cameraRight;
// 	glm::vec3 up;
// 	GLfloat pitch, yaw, roll; //óãëû êðåíà
// 	GLfloat lastX, lastY; //ïîëîæåíèå ìûøè
// public:
// 	Camera(GLFWwindow* win, glm::vec3 camPos, glm::vec3 camDir, glm::vec3 camUp);
// 	void keyboardMoveCamera(GLfloat deltaTime);
// 	void keyboardMoveMinecraftCamera(GLfloat deltaTime);
// 	void mouseTurnCamera(GLfloat deltaTime);
// 	glm::vec3 getPos();
// 	glm::vec3 getDir();
// 	glm::vec3 getUp();
// };
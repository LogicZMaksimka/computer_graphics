#include "Camera.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// void Camera::MinecraftKeyboardControl(GLfloat delta_time) {
// 	GLfloat cameraVelocity = 7 * delta_time;
// 	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
// 		cameraPos += cameraVelocity * cameraDir;
// 	}
// 	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
// 		cameraPos -= cameraVelocity * cameraDir;
// 	}
// 	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
// 		cameraPos -= glm::normalize(glm::cross(cameraUp, cameraDir)) * cameraVelocity; //èç-çà òàêîãî ôîðìàòà èãðîê íå ìîæåò ñìîòðåòü ñòðîãî ââåðõ
// 	}
// 	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
// 		cameraPos += glm::normalize(glm::cross(cameraUp, cameraDir)) * cameraVelocity;
// 	}
// 	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
// 		cameraPos -= cameraUp * cameraVelocity;
// 	}
// 	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
// 		cameraPos += cameraUp * cameraVelocity;
// 	}
// }

void Camera::UpdateFromKeyboard(GLfloat delta_time)
{
	GLfloat sensivity = movement_speed * delta_time;
	if (glfwGetKey(window.GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		pos += sensivity * dir;
	}
	if (glfwGetKey(window.GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		pos -= sensivity * dir;
	}
	if (glfwGetKey(window.GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		pos -= glm::normalize(glm::cross(up, dir)) * sensivity;
	}
	if (glfwGetKey(window.GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		pos += glm::normalize(glm::cross(up, dir)) * sensivity;
	}
	if (glfwGetKey(window.GetWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		pos -= up * sensivity;
	}
	if (glfwGetKey(window.GetWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		pos += up * sensivity;
	}
}

void Camera::UpdateFromMouse(GLfloat delta_time)
{
	GLfloat sensitivity = mouse_speed * delta_time;

	double x, y;
	glfwGetCursorPos(window.GetWindow(), &x, &y);

	GLfloat x_offset = (window.GetWidth() / 2) - x;
	GLfloat y_offset = (window.GetWidth() / 2) - y;

	pitch += y_offset * sensitivity;
	yaw += x_offset * sensitivity;

	// if (pitch >= 89.0f)
	// {
	// 	pitch = 89.0f;
	// }
	// if (pitch <= -89.0f)
	// {
	// 	pitch = -89.0f;
	// }
	GLfloat radYaw = glm::radians(yaw);

	dir = glm::vec3(cos(pitch) * cos(yaw),
					sin(pitch),
					cos(pitch) * sin(yaw));
}

void Camera::Update() {
	double delta_time = glfwGetTime() - last_update_time;
	UpdateFromKeyboard(delta_time);
	UpdateFromMouse(delta_time);
}

glm::mat4 Camera::GetViewMatrix()
{
	glm::mat4 view_matrix = glm::lookAt(
		pos, // camera position
		pos + dir, // position to look at
		up	 // tells where UP is. Defines rotation of camera over the direction of view
	);
	return view_matrix;
}

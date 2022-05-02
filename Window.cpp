#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Model.hpp"
#include "Camera.hpp"
#include "Window.hpp"

// glm::mat4 Window::CubeMVPMatrix()
// {
//   // Model transformations
//   glm::mat4 translate = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f));
//   glm::mat4 rotate = glm::rotate(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//   glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(1.0f));

//   return translate * rotate * scale;
// }

// // Camera transformations
// glm::mat4 Window::GetViewMatrix()
// {
//   GLfloat t = glfwGetTime();
//   glm::mat4 view_matrix = glm::lookAt(
//       glm::vec3(sin(t) * 5.0f, 0.0f, cos(t) * 5.0f), // camera position
//       glm::vec3(0.0f, 0.0f, 0.0f),                   // position to look at
//       glm::vec3(0.0f, 1.0f, 0.0f)                    // tells where UP is. Defines rotation of camera over the direction of view
//   );

//   return view_matrix;
// }

// // Homogeneous transformations
// glm::mat4 Window::GetProjectionMatrix(int width, int height)
// {
//   glm::mat4 projection_matrix = glm::perspective(
//       glm::radians(45.0f),          // FOV (field of view)
//       (float)width / float(height), // ratio of rectangle; determines camera's frustum longitudinal shape
//       0.1f,                         // determines distance to the near face of camera's frustum
//       100.0f                        // determines distance to the far side of camera's frustum
//   );

//   return projection_matrix;
// }

// Optimized for drawing copies of object
// This function binds VAO only once - before drawing all the objects
void Window::Draw(const Model &reference_model, const Camera &camera)
{
  glUseProgram(reference_model.shader_program); // use this shaders

  glBindVertexArray(reference_model.VAO); // use this VAO with according object

  glDrawArrays(GL_TRIANGLES, 0, reference_model.vertex_count); // - Draw triangle

  glBindVertexArray(0);
}

Window::Window(const char *title, int width_, int height_)
{
  width = width_;
  height = height_;

  // - Setup window
  glfwWindowHint(GLFW_SAMPLES, 4);               // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

  // - Create window
  window = glfwCreateWindow(width, height, title, NULL, NULL);
  if (!window)
  {
    std::cout << "glfwCreateWindow() Error";
    glfwTerminate();
    return;
  }

  // - Set window closing on ESC button
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
}
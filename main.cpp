#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <common/shader.hpp>
#include "vertex_arrays.hpp"
#include "Generate_VBO_VAO.hpp"
#include "Camera.hpp"
#include "Model.hpp"
#include "Window.hpp"


glm::mat4 GetModelMatrix()
{
  // Model transformations
  glm::mat4 translate = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f));
  glm::mat4 rotate = glm::rotate(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(1.0f));

  return translate * rotate * scale;
}


// Camera transformations
glm::mat4 GetViewMatrix()
{
  GLfloat t = glfwGetTime();
  glm::mat4 view_matrix = glm::lookAt(
      glm::vec3(sin(t) * 5.0f, 0.0f, cos(t) * 5.0f), // camera position
      glm::vec3(0.0f, 0.0f, 0.0f),                   // position to look at
      glm::vec3(0.0f, 1.0f, 0.0f)                    // tells where UP is. Defines rotation of camera over the direction of view
  );

  return view_matrix;
}

// Homogeneous transformations
glm::mat4 GetProjectionMatrix(float width, float height)
{
  glm::mat4 projection_matrix = glm::perspective(
      glm::radians(45.0f), // FOV (field of view)
      width / height,      // ratio of rectangle; determines camera's frustum longitudinal shape
      0.1f,                // determines distance to the near face of camera's frustum
      100.0f               // determines distance to the far side of camera's frustum
  );

  return projection_matrix;
}

int main()
{
  /// Init GLFW
  if (!glfwInit())
  {
    std::cout << "glfwInit() Error";
    return 1;
  }

  /// Create window

  Window window("main window", 1200, 800);
  window.MakeContextCurrent();

  // Вот тут (или ещё где-то, может в отдельном файле) создай модель куба и шара

  /// Init GLEW
  glewExperimental = true; // Needed for core profile
  if (glewInit() != GLEW_OK)
  {
    std::cout << "glewInit() Error";
    glfwTerminate();
    return 1;
  }

  /// Creating objects
  Model cube;
  cube.shader_program = LoadShaders("StandardVertexShader", "RedFragmentShader");
  cube.vertex_array = cube_vertex_array;
  cube.vertex_count = 6 * 2 * 3;
  cube.VBO = GenerateVBO();
  cube.VAO = GenerateVAO(cube.VBO, cube_vertex_array, sizeof(cube_vertex_array));

  // - Get matrix ID for interacting with GLSL matrix
  // GLuint glsl_time = glGetUniformLocation(cube.shader_program, "time");
  GLuint glsl_mvp_matrix = glGetUniformLocation(cube.shader_program, "mvp_matrix");


  /// Enable depth test
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS); // Accept fragment if it closer to the camera than the former one

  /// Enable color blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
  
  /// Draw loop

  // - Loop until the ESC key was pressed or the red cross was not pressed
  while (/*ESC button*/ glfwGetKey(window.GetWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS && /*red cross*/ !glfwWindowShouldClose(window.GetWindow()))
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear buffer to suppress flickering

    // Todo функция занимающаяся передачей разных параметров куба в шейдеры
    // glm::mat4 mvp_matrix = GetProjectionMatrix(window.GetWidth(), window.GetHeight()) * GetViewMatrix() * CubeMVPMatrix();
    // glUniformMatrix4fv(glsl_mvp_matrix, 1, GL_FALSE, &mvp_matrix[0][0]); // transfer mvp matrix to shader
    // glUniform1f(glsl_time, glfwGetTime());

    glm::mat4 mvp_matrix = GetProjectionMatrix(window.GetWidth(), window.GetHeight()) * GetViewMatrix() * GetModelMatrix();
    glUniformMatrix4fv(glsl_mvp_matrix, 1, GL_FALSE, &mvp_matrix[0][0]); // transfer mvp matrix to shader


    glUseProgram(cube.shader_program); // use this shaders

    glBindVertexArray(cube.VAO); // use this VAO with according object

    glDrawArrays(GL_TRIANGLES, 0, cube.vertex_count); // - Draw triangle

    glBindVertexArray(0);

    // - Check and call events and swap the buffers
    glfwSwapBuffers(window.GetWindow()); // swaps 2 buffers to avoid showing incomplete buffer
    glfwPollEvents();                    // checks for key pressing or mouse control
  }

  // Delete everything
  glDeleteBuffers(1, &cube.VBO);
  glDeleteVertexArrays(1, &cube.VAO);
  glDeleteProgram(cube.shader_program);
  glfwTerminate();

  return 0;
}
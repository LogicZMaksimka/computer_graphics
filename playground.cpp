#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <common/shader.hpp>
#include "vertex_arrays.h"

GLFWwindow *CreateWindow(int width, int height) {
  // - Setup window
  glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

  // - Create window
  GLFWwindow *window = glfwCreateWindow(width, height, "Main window", NULL, NULL);
  if (!window) {
    std::cout << "glfwCreateWindow() Error";
    glfwTerminate();
    return window;
  }

  // - Set drawing context to current window
  glfwMakeContextCurrent(window);

  // - Set window closing on ESC button
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  return window;
}

std::pair<GLuint, GLuint> Create_VBO_VAO(const GLfloat vertex_array[], int size) {
  GLuint VBO;
  GLuint VAO; // stores setting we set in for VBO
  glGenVertexArrays(1, &VAO); // gen 1 VAO object inside OpenGL library (in C we can't create custom objects)
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO); // use this VAO
  // _____________________________________________________________
  glBindBuffer(GL_ARRAY_BUFFER, VBO); // use this VBO
  glBufferData(GL_ARRAY_BUFFER, size, vertex_array, GL_STATIC_DRAW); // bind VBO and according array of vertexes
  glVertexAttribPointer( // tell OpenGL how to parse vertex buffer
      0, // just a random number used in shaders to determine which input corresponds to current buffer
      3, // array size
      GL_FLOAT, // array type
      GL_FALSE, // normalized
      0, // stride - step in which to read buffer
      (void *) 0 // array buffer offset - if data in buffer starts from some specific position
  );
  glEnableVertexAttribArray(0);
  // _____________________________________________________________
  glBindVertexArray(0);

  return std::make_pair(VBO, VAO);
}

glm::mat4 LeftTriangleMVPMatrix() {
  // Model transformations
  glm::mat4 translate = glm::translate(glm::mat4(), glm::vec3(-0.2f, 0.0f, 0.0f));
  glm::mat4 rotate = glm::rotate(glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(0.5f));

  return translate * rotate * scale;
}

glm::mat4 RightTriangleMVPMatrix() {
  // Model transformations
  glm::mat4 translate = glm::translate(glm::mat4(), glm::vec3(0.2f, 0.0f, 0.0f));
  glm::mat4 rotate = glm::rotate(glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(0.5f));

  return translate * rotate * scale;
}

// Camera transformations
glm::mat4 GetViewMatrix() {
  GLfloat t = glfwGetTime();
  glm::mat4 view_matrix = glm::lookAt(
      glm::vec3(sin(t) * 5.0f, 0.0f, cos(t) * 5.0f), // camera position
      glm::vec3(0.0f, 0.0f, 0.0f), // position to look at
      glm::vec3(0.0f, 1.0f, 0.0f) // tells where UP is. Defines rotation of camera over the direction of view
  );

  return view_matrix;
}

// Homogeneous transformations
glm::mat4 GetProjectionMatrix(float width, float height) {
  glm::mat4 projection_matrix = glm::perspective(
      glm::radians(45.0f), // FOV (field of view)
      width / height, // ratio of rectangle; determines camera's frustum longitudinal shape
      0.1f, // determines distance to the near face of camera's frustum
      100.0f // determines distance to the far side of camera's frustum
  );

  return projection_matrix;
}

void Draw(GLuint VAO,
          GLuint shader_program,
          GLuint glsl_mvp_matrix_access_object,
          int vertex_count,
          glm::mat4 mvp_matrix) {
  glUseProgram(shader_program); // use this shaders
  glBindVertexArray(VAO); // use this VAO with according object
  glUniformMatrix4fv(glsl_mvp_matrix_access_object, 1, GL_FALSE, &mvp_matrix[0][0]); // transfer mvp matrix to shader
  glDrawArrays(GL_TRIANGLES, 0, vertex_count); // - Draw triangle
  glBindVertexArray(0);
}

int main() {
  /// Init GLFW
  if (!glfwInit()) {
    std::cout << "glfwInit() Error";
    return 1;
  }

  /// Create window
  const int window_width = 1200;
  const int window_height = 800;
  GLFWwindow *window = CreateWindow(window_width, window_height);

  /// Init GLEW
  glewExperimental = true; // Needed for core profile
  if (glewInit() != GLEW_OK) {
    std::cout << "glewInit() Error";
    glfwTerminate();
    return 1;
  }

  /// Creating objects

  /// Create and compile GLSL shaders
  GLuint fractal_shader_program = LoadShaders("FractalVertexShader", "FractalFragmentShader");
  GLuint red_shader_program = LoadShaders("StandardVertexShader", "RedFragmentShader");
  GLuint green_shader_program = LoadShaders("StandardVertexShader", "GreenFragmentShader");
  // - Get matrix ID for interacting with GLSL matrix
  GLuint fractal_glsl_time = glGetUniformLocation(fractal_shader_program, "time");
  GLuint fractal_glsl_mvp_matrix = glGetUniformLocation(fractal_shader_program, "mvp_matrix");
  GLuint red_glsl_mvp_matrix = glGetUniformLocation(red_shader_program, "mvp_matrix");
  GLuint green_glsl_mvp_matrix = glGetUniformLocation(green_shader_program, "mvp_matrix");


  // - Saving vertex arrays to buffer, for OpenGL to be able to access it, when needed
  std::pair<GLuint, GLuint> triangle_VBO_VAO = Create_VBO_VAO(triangle_0, sizeof(triangle_0));
  std::pair<GLuint, GLuint> cube_VBO_VAO = Create_VBO_VAO(cube, sizeof(cube));
  std::pair<GLuint, GLuint> tetrahedron_VBO_VAO = Create_VBO_VAO(tetrahedron, sizeof(tetrahedron));

  glBindVertexArray(0); // unbind VAO

  /// Enable depth test
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS); // Accept fragment if it closer to the camera than the former one


  /// Enable color blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  /// Draw loop

  glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

  // - Loop until the ESC key was pressed or the red cross was not pressed
  while (/*ESC button*/glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
      && /*red cross*/!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear buffer to suppress flickering


    //__________________________________________________________________________________________________________________
    //                                          Draw Static Triangles
    //__________________________________________________________________________________________________________________
    // First triangle
//    glm::mat4 mvp_matrix = LeftTriangleMVPMatrix();
//    Draw(triangle_VBO_VAO.second, red_shader_program, red_glsl_mvp_matrix, 3, mvp_matrix);
//    // Second triangle
//    mvp_matrix = RightTriangleMVPMatrix();
//    Draw(triangle_VBO_VAO.second, green_shader_program, green_glsl_mvp_matrix, 3, mvp_matrix);
    //__________________________________________________________________________________________________________________
    //                                         Rotate around triangles
    //__________________________________________________________________________________________________________________
    // First triangle
//    glm::mat4 mvp_matrix = GetProjectionMatrix(window_width, window_height)
//        * GetViewMatrix()
//        * LeftTriangleMVPMatrix();
//    Draw(triangle_VBO_VAO.second, red_shader_program, red_glsl_mvp_matrix, 3, mvp_matrix);
//
//     // Second triangle
//    mvp_matrix = GetProjectionMatrix(window_width, window_height)
//        * GetViewMatrix()
//        * RightTriangleMVPMatrix();
//    Draw(triangle_VBO_VAO.second, green_shader_program, green_glsl_mvp_matrix, 3, mvp_matrix);
    //__________________________________________________________________________________________________________________
    //                                        Draw tetrahedron with fractals
    //__________________________________________________________________________________________________________________
    glUniform1f(fractal_glsl_time, glfwGetTime()); // pass time to vertex shader

    glm::mat4 mvp_matrix = GetProjectionMatrix(window_width, window_height) * GetViewMatrix();
    Draw(tetrahedron_VBO_VAO.second, fractal_shader_program, fractal_glsl_mvp_matrix, 4 * 3, mvp_matrix);


    // - Check and call events and swap the buffers
    glfwSwapBuffers(window); // swaps 2 buffers to avoid showing incomplete buffer
    glfwPollEvents(); // checks for key pressing or mouse control
  }

  glDeleteBuffers(1, &triangle_VBO_VAO.first);
  glDeleteBuffers(1, &cube_VBO_VAO.first);
  glDeleteBuffers(1, &tetrahedron_VBO_VAO.first);
  glDeleteVertexArrays(1, &triangle_VBO_VAO.second);
  glDeleteVertexArrays(1, &cube_VBO_VAO.second);
  glDeleteVertexArrays(1, &tetrahedron_VBO_VAO.second);
  glDeleteProgram(fractal_shader_program);
  glDeleteProgram(red_shader_program);
  glDeleteProgram(green_shader_program);

  glfwTerminate();

  return 0;
}
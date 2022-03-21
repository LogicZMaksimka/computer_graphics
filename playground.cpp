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
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE); //TODO: sticky keys ???

  return window;
}

GLuint CreateVBO(const GLfloat vertex_array[], int size) {
  GLuint VBO;
  glGenBuffers(1, &VBO); // generate one vertex buffer object
  glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind buffer to it's ID
  glBufferData(GL_ARRAY_BUFFER,
               size,
               vertex_array,
               GL_STATIC_DRAW); // save our vertex array to vertex buffer
  return VBO;
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

void Draw(GLuint VBO,
          GLuint shader_program,
          GLuint glsl_mvp_matrix_access_object,
          int vertex_count,
          glm::mat4 mvp_matrix) {
  // - Use shaders
  glUseProgram(shader_program);

  // - Tell OpenGL where and how to take vertexes from
  glEnableVertexAttribArray(0); // ?????????????????????????
  glBindBuffer(GL_ARRAY_BUFFER, VBO); // needs to be specified every time before drawing

  glVertexAttribPointer( // tell OpenGL how to parse vertex buffer
      0, // just a random number used in shaders to determine which input corresponds to current buffer
      3, // array size
      GL_FLOAT, // array type
      GL_FALSE, // normalized
      0, // stride - step in which to read buffer
      (void *) 0 // array buffer offset - if data in buffer starts from some specific position
  );

  // - Transfer mvp matrix to GLSL
  glUniformMatrix4fv(glsl_mvp_matrix_access_object, 1, GL_FALSE, &mvp_matrix[0][0]);

  // - Draw triangle
  // don't need to specify vertex array as we already gave OpenGl information about vertex buffer,
  // where all vertex data is stored

  glDrawArrays(GL_TRIANGLES, 0, vertex_count);
  glDisableVertexAttribArray(0); // ??????????????????????????

  // TODO: цвета в шейдере меняются в зависимости от времени
  // Todo: чекни shader toy или туториалы по ней
  // Todo: разобраться в коде загрузки шейдеров, а нужно ли????
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

  // - Create VAO (vertex array object) and bind actual array to it's id
  GLuint VAO;
  glGenVertexArrays(1, &VAO); // gen 1 VAO object inside OpenGL library (in C we can't create custom objects)
  glBindVertexArray(VAO); // bind newly created object to it's ID
  // Notice: The only option for OpenGl lib, written in C, is to use id's instead of actual objects as there is no OOP
  // and also it would be awful to make user create such object themselves as it ruins encapsulation rule


  /// Create and compile GLSL shaders
  GLuint fractal_shader_program = LoadShaders("FractalVertexShader", "FractalFragmentShader");
  GLuint red_shader_program = LoadShaders("StandartVertexShader", "RedFragmentShader");
  GLuint green_shader_program = LoadShaders("StandartVertexShader", "GreenFragmentShader");
  // - Get matrix ID for interacting with GLSL matrix
  GLuint fractal_glsl_time = glGetUniformLocation(fractal_shader_program, "time");
  GLuint fractal_glsl_mvp_matrix = glGetUniformLocation(fractal_shader_program, "mvp_matrix");
  GLuint red_glsl_mvp_matrix = glGetUniformLocation(red_shader_program, "mvp_matrix");
  GLuint green_glsl_mvp_matrix = glGetUniformLocation(green_shader_program, "mvp_matrix");


  // - Saving vertex arrays to buffer, for OpenGL to be able to access it, when needed
  GLuint triangle_VBO = CreateVBO(triangle_0, sizeof(triangle_0));
  GLuint cube_VBO = CreateVBO(cube, sizeof(cube));
  GLuint tetrahedron_VBO = CreateVBO(tetrahedron, sizeof(tetrahedron));

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
//    Draw(triangle_VBO, red_shader_program, red_glsl_mvp_matrix, 3, mvp_matrix);
//
//    // Second triangle
//    mvp_matrix = RightTriangleMVPMatrix();
//    Draw(triangle_VBO, green_shader_program, green_glsl_mvp_matrix, 3, mvp_matrix);
    //__________________________________________________________________________________________________________________
    //                                         Rotate around triangles
    //__________________________________________________________________________________________________________________
    // First triangle
    glm::mat4 mvp_matrix = GetProjectionMatrix(window_width, window_height)
        * GetViewMatrix()
        * LeftTriangleMVPMatrix();
    Draw(triangle_VBO, red_shader_program, red_glsl_mvp_matrix, 3, mvp_matrix);

    // Second triangle
    mvp_matrix = GetProjectionMatrix(window_width, window_height)
        * GetViewMatrix()
        * RightTriangleMVPMatrix();
    Draw(triangle_VBO, green_shader_program, green_glsl_mvp_matrix, 3, mvp_matrix);
    //__________________________________________________________________________________________________________________
    //                                        Draw tetrahedron with fractals
    //__________________________________________________________________________________________________________________
//    glUniform1f(fractal_glsl_time, glfwGetTime()); // pass time to vertex shader
//
//    glm::mat4 mvp_matrix = GetProjectionMatrix(window_width, window_height) * GetViewMatrix();
//    Draw(tetrahedron_VBO, red_shader_program, red_glsl_mvp_matrix, 4 * 3, mvp_matrix);


    // - Check and call events and swap the buffers
    glfwSwapBuffers(window); // swaps 2 buffers to avoid showing incomplete buffer
    glfwPollEvents(); // checks for key pressing or mouse control
  }

  glDeleteBuffers(1, &triangle_VBO);
  glDeleteBuffers(1, &cube_VBO);
  glDeleteBuffers(1, &tetrahedron_VBO);
  glDeleteVertexArrays(1, &VAO);
  glDeleteProgram(fractal_shader_program);
  glDeleteProgram(red_shader_program);
  glDeleteProgram(green_shader_program);

  glfwTerminate();

  return 0;
}
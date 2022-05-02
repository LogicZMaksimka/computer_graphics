// #include "Model.hpp"
// #include <GL/glew.h>

// void Create_VBO_VAO(const GLfloat vertex_array[], int size) {
//   GLuint VBO;
//   GLuint VAO;
//   glGenVertexArrays(1, &VAO); // gen 1 VAO object inside OpenGL library (in C we can't create custom objects)
//   glGenBuffers(1, &VBO);

//   glBindVertexArray(VAO); // use this VAO
//   // _____________________________________________________________
//   glBindBuffer(GL_ARRAY_BUFFER, VBO); // use this VBO
//   glBufferData(GL_ARRAY_BUFFER, size, vertex_array, GL_STATIC_DRAW); // bind VBO and according array of vertexes
//   glVertexAttribPointer( // tell OpenGL how to parse vertex buffer
//       0, // just a random number used in shaders to determine which input corresponds to current buffer
//       3, // array size
//       GL_FLOAT, // array type
//       GL_FALSE, // normalized
//       0, // stride - step in which to read buffer
//       (void *) 0 // array buffer offset - if data in buffer starts from some specific position
//   );
//   glEnableVertexAttribArray(0);
//   // _____________________________________________________________
//   glBindVertexArray(0);

//   return std::make_pair(VBO, VAO); // А точно ли вообще нам нужно VBO если VAO всё хранит ?
// }

// Model::~Model(){
//   glDeleteBuffers(1, &VBO);
//   glDeleteVertexArrays(1, &VAO);
//   glDeleteProgram(shader_program);
// }
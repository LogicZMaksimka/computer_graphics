#pragma once
#include <GL/glew.h>

GLuint GenerateVBO();

GLuint GenerateVAO(GLuint VBO, const GLfloat *vertex_array, int size);
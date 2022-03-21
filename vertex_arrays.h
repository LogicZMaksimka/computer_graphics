//
// Created by maksim on 19.03.2022.
//
#pragma once

#include "GL/glew.h"


// - Init vertex arrays with MODEL coordinates

static const GLfloat triangle_0[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
};

//static const GLfloat triangle_1[] = {
//    -0.5f, 0.5f, 0.0f,
//    0.5f, 0.0f, 0.0f,
//    -0.5f, -0.5f, 0.0f,
//};
//
//static const GLfloat triangle_2[] = {
//    0.5f, 0.5f, 0.0f,
//    -0.5f, 0.0f, 0.0f,
//    0.5f, -0.5f, 0.0f,
//};

static const GLfloat cube[] = {
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,

    1.0f, 1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,

    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,

    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,

    -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,

    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,

    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,

    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,

    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f
};

//static const GLfloat tetrahedron[] = {
//    1.0f, 0.0f, -1.0f, // 1
//    -1.0f, 0.0f, 1.0f, // 2
//    -1.0f, 0.0f, -1.0f, // 3
//
//    -1.0f, 0.0f, 1.0f, // 2
//    -1.0f, 0.0f, -1.0f, // 3
//    -1.0f, 1.0f, -1.0f // 4
//
//    -1.0f, -1.0f, 1.0f, // 2
//    -1.0f, 1.0f, -1.0f, // 4
//    1.0f, -1.0f, -1.0f, // 1
//
//    1.0f, -1.0f, -1.0f, // 1
//    -1.0f, -1.0f, -1.0f, // 3
//    -1.0f, 1.0f, -1.0f, // 4
//};

static const GLfloat tetrahedron[] = {
    0.0f, 0.0f, 1.0f, // 1
    0.0f, 0.942809f, -0.333333f, // 2
    -0.816497f, -0.471405f, -0.333333f, // 3

    -0.816497f, -0.471405f, -0.333333f, // 3
    0.816497f, -0.471405f, -0.333333f, // 4
    0.0f, 0.942809f, -0.333333f, // 2

    0.0f, 0.942809f, -0.333333f, // 2
    0.816497f, -0.471405f, -0.333333f, // 4
    0.0f, 0.0f, 1.0f, // 1

    0.0f, 0.0f, 1.0f, // 1
    -0.816497f, -0.471405f, -0.333333f, // 3
    0.816497f, -0.471405f, -0.333333f, // 4
};


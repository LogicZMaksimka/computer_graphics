#include "Model.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

class S
{
    public:
    int n = 42;   // default member initializer
    S() : n(7) {} // will set n to 7, not 42
};

class Projectile
{
private:
    // const Model &model;
    glm::vec3 pos;
    glm::vec3 dir;
    glm::vec3 up;

    float speed;

public:
    Projectile(const glm::vec3 &pos_,
               const glm::vec3 &dir_,
               const glm::vec3 &up_ = glm::vec3(0.0f, 1.0f, 0.0f),
               float speed_ = 1.0) : pos(pos_), dir(dir_), up(up_), speed(speed_) {}
};

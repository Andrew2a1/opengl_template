#pragma once

#include <GL/freeglut.h>

#include <vector>

class Spring
{
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

public:
    void create_vertices(GLfloat rounds, GLfloat height, GLfloat thickness, GLfloat radius);
    void draw() const;
};
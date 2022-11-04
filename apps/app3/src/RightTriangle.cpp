#include "RightTriangle.h"

#include <GL/freeglut.h>

#include <glm/gtc/type_ptr.hpp>

RightTriangle::RightTriangle(const glm::vec2 &pos, const glm::vec3 &color) : pos(pos), color(color) {}

glm::vec2 RightTriangle::center() const
{
    const glm::vec2 v2{pos.x, pos.y + TRIANGLE_SIZE};
    const glm::vec2 v3{pos.x + TRIANGLE_SIZE, pos.y};
    return (pos + v2 + v3) / 3.0f;
}

void RightTriangle::draw() const
{
    glPushMatrix();
    glTranslated(pos.x, pos.y, 0.0f);
    glRotated(rotation, 0.0f, 0.0f, -1.0f);
    glBegin(GL_POLYGON);
    glColor3fv(glm::value_ptr(color));
    glVertex2f(0.0f, 0.0f);
    glColor3fv(glm::value_ptr(color));
    glVertex2f(0.0f, TRIANGLE_SIZE);
    glColor3fv(glm::value_ptr(color));
    glVertex2f(TRIANGLE_SIZE, 0.0f);
    glEnd();
    glPopMatrix();
}

void RightTriangle::set_pos(const glm::vec2 &pos) { this->pos = pos; }

void RightTriangle::set_rotation(float rotation) { this->rotation = rotation; }

float RightTriangle::get_rotation() const { return rotation; }

const glm::vec2 &RightTriangle::get_pos() const { return pos; }
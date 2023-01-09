#include "Pendulum.h"

#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

#include <cmath>

constexpr double g = 9.81;

Pendulum::Pendulum(const glm::vec3& position) : position(position)
{
    GLUquadricObj* sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricTexture(sphere, true);
    gluQuadricNormals(sphere, GLU_SMOOTH);
    sphereID = glGenLists(1);
    glNewList(sphereID, GL_COMPILE);
    gluSphere(sphere, ball_radius, 32, 32);
    glEndList();
    gluDeleteQuadric(sphere);

    GLUquadricObj* cylinder = gluNewQuadric();
    gluQuadricDrawStyle(cylinder, GLU_FILL);
    gluQuadricTexture(cylinder, true);
    gluQuadricNormals(cylinder, GLU_SMOOTH);
    cylinderID = glGenLists(1);
    glNewList(cylinderID, GL_COMPILE);
    gluCylinder(cylinder, 0.03f, 0.03f, arm_length, 32, 32);
    glEndList();
    gluDeleteQuadric(cylinder);
}

void Pendulum::update(double dt)
{
    const double angular_acceleration = ball_mass * g * -std::sin(swing_angle) / arm_length;
    swing_angle += angular_speed * dt;
    angular_speed = (angular_speed + angular_acceleration * dt) * damping;

    if (swing_angle < -M_PI_2)
    {
        swing_angle = -M_PI_2;
        angular_speed = 0;
    }
    else if (swing_angle > M_PI_2)
    {
        swing_angle = M_PI_2;
        angular_speed = 0;
    }
}

void Pendulum::collide(Pendulum& other)
{
    const double self_velocity =
        (this->angular_speed * (this->ball_mass - other.ball_mass) + 2 * other.ball_mass * other.angular_speed) / (this->ball_mass + other.ball_mass);
    const double other_velocity =
        (other.angular_speed * (other.ball_mass - this->ball_mass) + 2 * this->ball_mass * this->angular_speed) / (this->ball_mass + other.ball_mass);

    this->set_angular_speed(self_velocity);
    other.set_angular_speed(other_velocity);

    const auto self_position = this->get_ball_position();
    const auto other_position = other.get_ball_position();
    if (self_position.x < other_position.x)
    {
        this->swing_angle -= 0.01;
    }
    else
    {
        this->swing_angle += 0.01;
    }
}

bool Pendulum::is_colliding(const Pendulum& other) const
{
    if (glm::distance(this->get_ball_position(), other.get_ball_position()) <= this->ball_radius + other.ball_radius)
    {
        return true;
    }
    return false;
}

glm::vec3 Pendulum::get_ball_position() const { return glm::vec3{std::sin(swing_angle) * arm_length, -std::cos(swing_angle) * arm_length, 0} + position; }

void Pendulum::draw() const
{
    glPushMatrix();
    glTranslated(position.x - 10, position.y, position.z);
    glRotated(swing_angle * 180 / M_PI, 0, 0, 1);

    /* Top bar */
    glPushMatrix();
    glTranslated(0, 0, -arm_length / 2);
    glCallList(cylinderID);
    glPopMatrix();

    /* Arm */
    glPushMatrix();
    glRotated(90, 1.0, 0.0, 0.0);
    glCallList(cylinderID);
    glPopMatrix();

    /* Ball */
    glPushMatrix();
    glTranslated(0, -arm_length, 0);
    glCallList(sphereID);
    glPopMatrix();
    glPopMatrix();
}
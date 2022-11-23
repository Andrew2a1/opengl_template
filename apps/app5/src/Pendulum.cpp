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
    gluQuadricTexture(sphere, TRUE);
    gluQuadricNormals(sphere, GLU_SMOOTH);
    sphereID = glGenLists(1);
    glNewList(sphereID, GL_COMPILE);
    gluSphere(sphere, ball_radius, 32, 32);
    glEndList();
    gluDeleteQuadric(sphere);

    GLUquadricObj* cylinder = gluNewQuadric();
    gluQuadricDrawStyle(cylinder, GLU_FILL);
    gluQuadricTexture(cylinder, TRUE);
    gluQuadricNormals(cylinder, GLU_SMOOTH);
    cylinderID = glGenLists(1);
    glNewList(cylinderID, GL_COMPILE);
    gluCylinder(cylinder, 0.03f, 0.03f, arm_length, 32, 32);
    glEndList();
    gluDeleteQuadric(cylinder);
}

void Pendulum::update(double dt)
{
    constexpr double damping = 0.996;
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

    // const auto self_position = this->get_ball_position();
    // const auto other_position = other.get_ball_position();
    // const auto new_position =
    //     other_position + glm::normalize(other_position - self_position) * static_cast<float>((this->ball_radius + other.ball_radius) * 1.001);

    // double new_angle = std::atan2(new_position.x, -new_position.y) - M_PI_2;
    // std::cout << "x: " << new_position.x << ", y: " << new_position.y << ", angle: " << new_angle << std::endl;
    // if (!std::isnan(new_angle)) swing_angle = new_angle;
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

    glPushMatrix();
    glRotated(90, 1.0, 0.0, 0.0);
    glCallList(cylinderID);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, -arm_length, 0);
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, sphere_texture);
    glCallList(sphereID);
    // glBindTexture(GL_TEXTURE_2D, NULL);
    glPopMatrix();

    glPopMatrix();
}
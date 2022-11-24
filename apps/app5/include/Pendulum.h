#pragma once

// clang-format off
#include <gl/glew.h>
// clang-format on

#include <GL/freeglut.h>

#include <glm/glm.hpp>

class Pendulum
{
private:
    glm::vec3 position;
    GLuint sphereID = 0;
    GLuint cylinderID = 0;

    double arm_length = 2;
    double ball_radius = 0.5;
    double ball_mass = 2;
    double damping = 0.996;

    double swing_angle = 0;
    double angular_speed = 0;

public:
    explicit Pendulum(const glm::vec3 &position = glm::vec3(0.0f));

    bool is_colliding(const Pendulum &other) const;
    void collide(Pendulum &other);

    void update(double dt);
    void draw() const;

    glm::vec3 get_ball_position() const;
    double get_ball_radius() const { return ball_radius; }
    double get_ball_mass() const { return ball_mass; }
    double get_swing_angle() const { return swing_angle; }
    void set_angular_speed(double speed) { angular_speed = speed; }
    void set_swing_angle(double angle) { swing_angle = angle; }
    void set_damping(double new_damping) { damping = new_damping; }
};
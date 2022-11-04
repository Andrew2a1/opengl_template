#pragma once

#include <glm/glm.hpp>

constexpr float TRIANGLE_SIZE = 0.25f;

class RightTriangle
{
private:
    float rotation = 0.0f;
    glm::vec2 pos;
    glm::vec3 color;

public:
    explicit RightTriangle(const glm::vec2 &pos = {}, const glm::vec3 &color = {1.0f, 0.0f, 0.0f});

    glm::vec2 center() const;
    void draw() const;

    void set_pos(const glm::vec2 &pos);
    void set_rotation(float rotation);
    float get_rotation() const;
    const glm::vec2 &get_pos() const;
};

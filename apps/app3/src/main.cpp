#include <GL/freeglut.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <thread>
#include <vector>

#include "RightTriangle.h"

constexpr float ROTATION_TIME_S = 8.0f;         // 8 seconds per entire rotation
constexpr float UPDATE_DELAY_S = 1.0f / 60.0f;  // 60FPS
constexpr float MOVEMENT_SPEED = 3 * TRIANGLE_SIZE / (10.0f * ROTATION_TIME_S);

std::vector<RightTriangle> triangles;
float common_rotation = 0;

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glLoadIdentity();
    glRotated(common_rotation, 0, 0, 1.0);

    for (int i = 0; i < 4; ++i)
    {
        glRotated(90 * i, 0, 0, 1.0);
        for (const auto& triangle : triangles)
        {
            triangle.draw();
        }
    }

    glPopMatrix();

    glFlush();
    glutSwapBuffers();
}

void Reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDrawBuffer(GL_BACK);
}

void update_callback(int)
{
    const float common_rotation_delta = 360.0f * UPDATE_DELAY_S / ROTATION_TIME_S;
    common_rotation += common_rotation_delta;

    for (auto& triangle : triangles)
    {
        const glm::vec2 direction = glm::normalize(triangle.center());
        triangle.set_pos(triangle.get_pos() + direction * UPDATE_DELAY_S * MOVEMENT_SPEED);
        triangle.set_rotation(triangle.get_rotation() + 3.0f * common_rotation_delta);
    }

    glutPostRedisplay();
    glutTimerFunc(static_cast<int>(UPDATE_DELAY_S * 1000.0f), update_callback, 0);
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Template");

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    // Generate triangles
    const std::vector<glm::vec3> colors = {{0.0f, 0.0f, 1.0f}, {1.0f, 0.4f, 0.0f}, {1.0f, 0.0f, 1.0f},
                                           {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}};
    int color_no = 0;

    for (int x = 0; x < 3; ++x)
    {
        for (int y = 0; y < 3 - x; ++y)
        {
            triangles.emplace_back(glm::vec2{y * TRIANGLE_SIZE, x * TRIANGLE_SIZE}, colors[color_no++]);
        }
    }

    // Set callbacks

    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutTimerFunc(static_cast<int>(UPDATE_DELAY_S * 1000.0f), update_callback, 0);
    glutMainLoop();
    return 0;
}

// clang-format off
#include <gl/glew.h>
// clang-format on

#include <GL/freeglut.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <thread>
#include <vector>

#include "Pendulum.h"
#include "Texture.h"

constexpr float UPDATE_DELAY_S = 1.0f / 60.0f;  // 60FPS

double translate_y = 1.5;
double translate_x = 0;
double translate_z = -12.0;
double rotate_x = 0;
double rotate_y = 0;

double L = 2;
double sphere_angle = 3.14 / 4;
double angular_speed = 0;

bool is_left_btn_pressed = false;
bool is_middle_btn_pressed = false;

glm::vec2 prev_left_mouse_pos{0, 0};
glm::vec2 prev_middle_mouse_pos{0, 0};

GLfloat LightAmbient[] = {0.1f, 0.1f, 0.1f, 0.1f};
GLfloat LightDiffuse[] = {0.7f, 0.7f, 0.7f, 0.7f};
GLfloat LightSpecular[] = {0.0f, 0.0f, 0.0f, 0.1f};
GLfloat LightPosition[] = {5.0f, 5.0f, 5.0f, 0.0f};

std::vector<Pendulum> pendulums;

void Display()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(translate_x, translate_y, translate_z);
    glRotated(rotate_x, 1.0, 0.0, 0.0);
    glRotated(rotate_y, 0.0, 1.0, 0.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, NULL);

    glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
    glEnable(GL_LIGHT0);

    glPushMatrix();
    glScaled(4.0, 0.2, 2.0);
    glutSolidCube(1.0);
    glPopMatrix();

    for (const auto& pendulum : pendulums)
    {
        pendulum.draw();
    }

    glFlush();
    glutSwapBuffers();
}

void Reshape(int width, int height)
{
    double aspect;
    if (height == 0)
    {
        aspect = (GLdouble)width;
    }
    else
    {
        aspect = (GLdouble)width / (GLdouble)height;
    }
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45, aspect, 1, 80.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void update_callback(int)
{
    for (auto& pendulum : pendulums)
    {
        pendulum.update(UPDATE_DELAY_S);
    }
    for (int i = 0; i < pendulums.size(); ++i)
    {
        for (int j = i + 1; j < pendulums.size(); ++j)
        {
            if (pendulums[i].is_colliding(pendulums[j]))
            {
                pendulums[i].collide(pendulums[j]);
            }
        }
    }
    glutPostRedisplay();
    glutTimerFunc(static_cast<int>(UPDATE_DELAY_S * 1000.0f), update_callback, 0);
}

void mouse_callback(int button, int state, int x, int y)
{
    constexpr int SCROOL_UP = 3;
    constexpr int SCROOL_DOWN = 4;

    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            prev_left_mouse_pos = {x, y};
            is_left_btn_pressed = true;
        }
        else
        {
            is_left_btn_pressed = false;
        }
    }
    else if (button == GLUT_MIDDLE_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            prev_middle_mouse_pos = {x, y};
            is_middle_btn_pressed = true;
        }
        else
        {
            is_middle_btn_pressed = false;
        }
    }
    else if (button == SCROOL_UP)
    {
        translate_z += 0.1;
    }
    else if (button == SCROOL_DOWN)
    {
        translate_z -= 0.1;
    }
}

void mouse_move_callback(int x, int y)
{
    const glm::vec2 pos{x, y};
    if (is_left_btn_pressed)
    {
        const glm::vec2 delta = prev_left_mouse_pos - pos;
        prev_left_mouse_pos = pos;
        translate_x -= delta.x / 64.0;
        translate_y += delta.y / 64.0;
        glutPostRedisplay();
    }

    if (is_middle_btn_pressed)
    {
        const glm::vec2 delta = prev_middle_mouse_pos - pos;
        prev_middle_mouse_pos = pos;
        rotate_y = rotate_y - delta.x;
        rotate_x = rotate_x - delta.y / 3.0;
        glutPostRedisplay();
    }
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Template");
    glewInit();

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    for (int i = 0; i < 5; ++i)
    {
        pendulums.emplace_back(glm::vec3{(i + 5) * 1.5, 0, 0});
    }

    pendulums[4].set_swing_angle(3.14 / 2);

    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutMouseFunc(mouse_callback);
    glutMotionFunc(mouse_move_callback);
    glutTimerFunc(static_cast<int>(UPDATE_DELAY_S * 1000.0f), update_callback, 0);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    glutMainLoop();
    return 0;
}

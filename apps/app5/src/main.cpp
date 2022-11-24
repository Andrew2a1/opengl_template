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
float simulation_speed = 1;

double translate_y = 1.5;
double translate_x = 0;
double translate_z = -12.0;
double rotate_x = 0;
double rotate_y = 0;

GLuint cylinderID, sphereID;
constexpr double cylinder_len = 7.1;

bool is_left_btn_pressed = false;
bool is_middle_btn_pressed = false;
bool is_right_btn_pressed = false;

glm::vec2 prev_left_mouse_pos{0, 0};
glm::vec2 prev_middle_mouse_pos{0, 0};
glm::vec2 prev_right_mouse_pos{0, 0};

GLfloat LightAmbient[] = {0.2f, 0.2f, 0.2f, 0.2f};
GLfloat LightDiffuse[] = {0.8f, 0.8f, 0.8f, 0.8f};
GLfloat LightSpecular[] = {0.0f, 0.0f, 0.0f, 0.1f};
GLfloat LightPosition[] = {5.0f, 5.0f, 5.0f, 0.0f};

std::vector<Pendulum> pendulums;

GLuint wood_texture = 0;
GLuint metal_texture = 0;
GLuint steel_texture = 0;

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

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, metal_texture);

    /* Top */
    glPushMatrix();
    glTranslated(-cylinder_len / 2 + 0.4, 0.0, 1);
    glRotated(90, 0.0, 1.0, 0.0);
    glCallList(cylinderID);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-cylinder_len / 2 + 0.4, 0.0, -1);
    glRotated(90, 0.0, 1.0, 0.0);
    glCallList(cylinderID);
    glPopMatrix();

    /* Sides */

    glPushMatrix();
    glScaled(1, 0.5, 1);
    glTranslated(4, 0, 1);
    glRotated(90, 1.0, 0.0, 0.0);
    glCallList(cylinderID);
    glPopMatrix();

    glPushMatrix();
    glScaled(1, 0.5, 1);
    glTranslated(-3.1, 0, 1);
    glRotated(90, 1.0, 0.0, 0.0);
    glCallList(cylinderID);
    glPopMatrix();

    glPushMatrix();
    glScaled(1, 0.5, -1);
    glTranslated(4, 0, 1);
    glRotated(90, 1.0, 0.0, 0.0);
    glCallList(cylinderID);
    glPopMatrix();

    glPushMatrix();
    glScaled(1, 0.5, 1);
    glTranslated(-3.1, 0, -1);
    glRotated(90, 1.0, 0.0, 0.0);
    glCallList(cylinderID);
    glPopMatrix();

    /* Soft ends */

    glPushMatrix();
    glTranslated(4, 0, 1);
    glCallList(sphereID);
    glPopMatrix();

    glPushMatrix();
    glTranslated(4, 0, -1);
    glCallList(sphereID);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-3.1, 0, 1);
    glCallList(sphereID);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-3.1, 0, -1);
    glCallList(sphereID);
    glPopMatrix();

    /* Table */

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, wood_texture);

    glPushMatrix();
    glTranslated(-5, -3.4, -5);
    glRotated(90, 1, 0, 0);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2i(0.0f, 10.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2i(0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2i(10.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2i(10.0f, 10.0f);
    glEnd();
    glPopMatrix();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, steel_texture);

    /* Pendulums */
    for (const auto& pendulum : pendulums)
    {
        pendulum.draw();
    }

    glBindTexture(GL_TEXTURE_2D, NULL);

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
    if (!is_right_btn_pressed)
    {
        for (auto& pendulum : pendulums)
        {
            pendulum.update(simulation_speed * UPDATE_DELAY_S);
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
    else if (button == GLUT_RIGHT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            prev_right_mouse_pos = {x, y};
            is_right_btn_pressed = true;
        }
        else
        {
            is_right_btn_pressed = false;
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

    if (is_right_btn_pressed)
    {
        const glm::vec2 delta = prev_right_mouse_pos - pos;
        prev_right_mouse_pos = pos;
        pendulums[4].set_swing_angle(pendulums[4].get_swing_angle() - delta.x / 64.0);
        glutPostRedisplay();
    }
}

void keyboard_arrows_callback(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_UP:
            simulation_speed += 0.01f;
            break;
        case GLUT_KEY_DOWN:
            simulation_speed -= 0.01f;
            break;
        default:
            return;
    }
    std::cout << "Simulation speed: " << simulation_speed << std::endl;
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Template");
    glewInit();

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    wood_texture = LoadTexture("wood.bmp");
    metal_texture = LoadTexture("metal.bmp");
    steel_texture = LoadTexture("steel.bmp");

    for (int i = 0; i < 5; ++i)
    {
        pendulums.emplace_back(glm::vec3{(i + 5) * 1.5, 0, 0});
    }

    GLUquadricObj* cylinder = gluNewQuadric();
    gluQuadricDrawStyle(cylinder, GLU_FILL);
    gluQuadricTexture(cylinder, TRUE);
    gluQuadricNormals(cylinder, GLU_SMOOTH);
    cylinderID = glGenLists(1);
    glNewList(cylinderID, GL_COMPILE);
    gluCylinder(cylinder, 0.06f, 0.06f, cylinder_len, 32, 32);
    glEndList();
    gluDeleteQuadric(cylinder);

    GLUquadricObj* sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricTexture(sphere, TRUE);
    gluQuadricNormals(sphere, GLU_SMOOTH);
    sphereID = glGenLists(1);
    glNewList(sphereID, GL_COMPILE);
    gluSphere(sphere, 0.1, 32, 32);
    glEndList();
    gluDeleteQuadric(sphere);

    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutSpecialFunc(keyboard_arrows_callback);
    glutMouseFunc(mouse_callback);
    glutMotionFunc(mouse_move_callback);
    glutTimerFunc(static_cast<int>(UPDATE_DELAY_S * 1000.0f), update_callback, 0);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    glutMainLoop();
    return 0;
}

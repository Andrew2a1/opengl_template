// clang-format off
#include <GL/glew.h>
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

#include "Spring.h"
#include "Texture.h"

constexpr float UPDATE_DELAY_S = 1.0f / 60.0f;  // 60FPS

double translate_y = 1.5;
double translate_x = 0;
double translate_z = -8.0;
double rotate_x = 0;
double rotate_y = 0;

double spring_x = 1;
double ball_velocity = 0;

bool is_left_btn_pressed = false;
bool is_middle_btn_pressed = false;

glm::vec2 prev_left_mouse_pos{0, 0};
glm::vec2 prev_middle_mouse_pos{0, 0};

GLfloat LightAmbient[] = {0.1f, 0.1f, 0.1f, 0.1f};
GLfloat LightDiffuse[] = {0.7f, 0.7f, 0.7f, 0.7f};
GLfloat LightSpecular[] = {0.0f, 0.0f, 0.0f, 0.1f};
GLfloat LightPosition[] = {5.0f, 5.0f, 5.0f, 0.0f};

GLUquadricObj* quadratic = gluNewQuadric();
unsigned int sphereID, cylinderID;
Spring spring;

GLuint sphere_texture = 0;
GLuint spring_texture = 0;

void Display()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(translate_x, translate_y, translate_z);
    glRotated(rotate_x, 1.0, 0.0, 0.0);
    glRotated(rotate_y, 0.0, 1.0, 0.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, 0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
    glEnable(GL_LIGHT0);

    glPushMatrix();
    glScaled(4.0, 0.2, 2.0);
    glutSolidCube(1.0);
    glPopMatrix();

    /* Górne połączenie sprężyny */
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, spring_texture);

    glPushMatrix();
    glRotated(90, 0.0, 1.0, 0.0);
    glTranslated(0.0, -0.2, 0.0);
    glCallList(cylinderID);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.0, -0.2, 0.0);
    glutSolidSphere(0.03, 6, 6);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.0, 0.05, 0.0);
    glRotated(90, 1.0, 0.0, 0.0);
    glCallList(cylinderID);
    glPopMatrix();
    /* --- */

    /* Sprężyna */
    glPushMatrix();
    glTranslated(0.0, -0.2, 0.0);
    glRotated(90, 1.0, 0.0, 0.0);
    spring.draw();
    glPopMatrix();
    /* --- */

    /* Dolne połączenie sprężyny */
    glPushMatrix();
    glTranslated(0.0, -2.2 - spring_x, 0.0);
    glRotated(90, 0.0, 1.0, 0.0);
    glCallList(cylinderID);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.0, -2.2 - spring_x, 0.0);
    glutSolidSphere(0.03, 6, 6);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.0, -2.2 - spring_x, 0.0);
    glRotated(90, 1.0, 0.0, 0.0);
    glCallList(cylinderID);
    glPopMatrix();
    /* --- */

    /* Kula */
    glPushMatrix();
    glTranslated(0.0, -2.8 - spring_x, 0.0);
    glRotated(90, 1.0, 0.0, 0.0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sphere_texture);
    glCallList(sphereID);
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
    /* --- */

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
    constexpr double ball_mass = 0.2;
    constexpr double g = 9.81;
    constexpr double k = 5.2;
    constexpr double weight_force = ball_mass * g;

    const double spring_force = -k * spring_x;
    const double result_force = spring_force - weight_force;

    ball_velocity += result_force / ball_mass * UPDATE_DELAY_S;
    spring_x += ball_velocity * UPDATE_DELAY_S;

    spring.create_vertices(12, 2 + (float)spring_x, 0.03f, 0.25f);

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

    sphere_texture = LoadTexture("wood.bmp");
    spring_texture = LoadTexture("metal.bmp");
    spring.create_vertices(12, 2, 0.03f, 0.25f);

    // Generowanie kuli
    GLUquadricObj* sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricTexture(sphere, true);
    gluQuadricNormals(sphere, GLU_SMOOTH);
    sphereID = glGenLists(1);
    glNewList(sphereID, GL_COMPILE);
    gluSphere(sphere, 0.5, 32, 32);
    glEndList();
    gluDeleteQuadric(sphere);

    // Generowanie walcow
    GLUquadricObj* cylinder = gluNewQuadric();
    gluQuadricDrawStyle(cylinder, GLU_FILL);
    gluQuadricTexture(cylinder, true);
    gluQuadricNormals(cylinder, GLU_SMOOTH);
    cylinderID = glGenLists(1);
    glNewList(cylinderID, GL_COMPILE);
    gluCylinder(cylinder, 0.03f, 0.03f, 0.25, 32, 32);
    glEndList();
    gluDeleteQuadric(cylinder);

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

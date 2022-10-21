#include <GL/freeglut.h>

#include <cmath>
#include <glm/glm.hpp>
#include <iostream>
#include <algorithm>

enum GLDisplayListNames
{
    ArmPart = 1
};

double translate_y = 0;
double translate_x = 0;
double angle_1 = 15;
double angle_2 = 15;
double angle_3 = 0;
double angle_4 = 0;

bool is_left_btn_pressed = false;
bool is_right_btn_pressed = false;
bool is_middle_btn_pressed = false;

glm::vec2 prev_left_mouse_pos{0, 0};
glm::vec2 prev_right_mouse_pos{0, 0};
glm::vec2 prev_middle_mouse_pos{0, 0};

GLfloat RedSurface[] = {1.0f, 0.0f, 0.0f, 1.0f};
GLfloat GreenSurface[] = {0.0f, 1.0f, 0.0f, 1.0f};
GLfloat BlueSurface[] = {0.0f, 0.0f, 1.0f, 1.0f};
GLfloat LightAmbient[] = {0.1f, 0.1f, 0.1f, 0.1f};
GLfloat LightDiffuse[] = {0.7f, 0.7f, 0.7f, 0.7f};
GLfloat LightSpecular[] = {0.0f, 0.0f, 0.0f, 0.1f};
GLfloat LightPosition[] = {5.0f, 5.0f, 5.0f, 0.0f};

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
    glEnable(GL_LIGHT0);

    glPushMatrix();
    glTranslated(translate_x, translate_y, -9.0);
    glRotated(angle_1, 0, 0, 1);
    glRotated(angle_3, 0, 1, 0);
    glRotated(angle_4, 1, 0, 0);
    glPushMatrix();
    glTranslated(1, 0, 0);
    glRotated(angle_2, 0, 0, 1);
    glScaled(1, 0.5, 0.5);
    glCallList(ArmPart);
    glPopMatrix();
    glScaled(1, 0.5, 0.5);
    glCallList(ArmPart);
    glPopMatrix();

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

    gluPerspective(45, aspect, 1, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDrawBuffer(GL_BACK);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}

void mouse_callback(int button, int state, int x, int y)
{
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
}

void mouse_move_callback(int x, int y)
{
    const glm::vec2 pos{x, y};
    if (is_left_btn_pressed)
    {
        const glm::vec2 delta = prev_left_mouse_pos - pos;
        prev_left_mouse_pos = pos;
        translate_x -= delta.x / 32.0;
        translate_y += delta.y / 32.0;
        Display();
    }

    if (is_right_btn_pressed)
    {
        const glm::vec2 delta = prev_right_mouse_pos - pos;
        prev_right_mouse_pos = pos;
        angle_2 = std::min(std::max(angle_2 - delta.x / 3.0, -90.0), 90.0);
        angle_1 = std::min(std::max(angle_1 - delta.y / 3.0, -180.0), 180.0);
        Display();
    }

    if (is_middle_btn_pressed)
    {
        const glm::vec2 delta = prev_middle_mouse_pos - pos;
        prev_middle_mouse_pos = pos;
        angle_3 = angle_3 - delta.x;
        angle_4 = angle_4 - delta.y / 3.0;
        Display();
    }
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Template");

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    glNewList(ArmPart, GL_COMPILE);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, RedSurface);
    glBegin(GL_POLYGON);
    glNormal3d(1.0, 0.0, 0.0);
    glVertex3d(1.0, 1.0, 1.0);
    glVertex3d(1.0, -1.0, 1.0);
    glVertex3d(1.0, -1.0, -1.0);
    glVertex3d(1.0, 1.0, -1.0);
    glEnd();
    glBegin(GL_POLYGON);
    glNormal3d(-1.0, 0.0, 0.0);
    glVertex3d(-1.0, -1.0, 1.0);
    glVertex3d(-1.0, 1.0, 1.0);
    glVertex3d(-1.0, 1.0, -1.0);
    glVertex3d(-1.0, -1.0, -1.0);
    glEnd();
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, GreenSurface);
    glBegin(GL_POLYGON);
    glNormal3d(0.0, 1.0, 0.0);
    glVertex3d(1.0, 1.0, 1.0);
    glVertex3d(-1.0, 1.0, 1.0);
    glVertex3d(-1.0, 1.0, -1.0);
    glVertex3d(1.0, 1.0, -1.0);
    glEnd();
    glBegin(GL_POLYGON);
    glNormal3d(0.0, -1.0, 0.0);
    glVertex3d(-1.0, -1.0, 1.0);
    glVertex3d(1.0, -1.0, 1.0);
    glVertex3d(1.0, -1.0, -1.0);
    glVertex3d(-1.0, -1.0, -1.0);
    glEnd();
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, BlueSurface);
    glBegin(GL_POLYGON);
    glNormal3d(0.0, 0.0, 1.0);
    glVertex3d(1.0, 1.0, 1.0);
    glVertex3d(-1.0, 1.0, 1.0);
    glVertex3d(-1.0, -1.0, 1.0);
    glVertex3d(1.0, -1.0, 1.0);
    glEnd();
    glBegin(GL_POLYGON);
    glNormal3d(0.0, 0.0, -1.0);
    glVertex3d(-1.0, 1.0, -1.0);
    glVertex3d(1.0, 1.0, -1.0);
    glVertex3d(1.0, -1.0, -1.0);
    glVertex3d(-1.0, -1.0, -1.0);
    glEnd();
    glEndList();

    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutMouseFunc(mouse_callback);
    glutMotionFunc(mouse_move_callback);

    glutMainLoop();
    return 0;
}

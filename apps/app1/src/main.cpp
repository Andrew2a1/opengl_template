#include <GL/freeglut.h>
#include <iostream>

void Display()
{
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POLYGON);
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
    glVertex2f(100.0f, 50.0f);
    glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
    glVertex2f(450.0f, 400.0f);
    glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
    glVertex2f(450.0f, 50.0f);
    glEnd();
    glFlush();
    glutSwapBuffers();
}

void Reshape(int cx, int cy)
{
    GLsizei width, height;
    GLdouble aspect;
    width = cx;
    height = cy;
    if (cy == 0)
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
    gluOrtho2D(0.0, 500.0 * aspect, 0.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Template");

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);

    glutMainLoop();
    return 0;
}
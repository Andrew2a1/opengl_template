#include <GL/freeglut.h>
#include <iostream>

void Display()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 0.0, 1.0);
    glBegin(GL_POLYGON);

    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);
    glVertex3f(1.0, 1.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);

    glEnd();
    glFlush();
    glutSwapBuffers();
}

void Reshape(int, int)
{
    Display();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Template");

    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);

    std::cout << "OpenGL version supported by this platform: " << glGetString(GL_VERSION) << std::endl;

    glutMainLoop();
    return 0;
}
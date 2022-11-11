#include "Spring.h"

#define _USE_MATH_DEFINES
#include <math.h>

void Spring::create_vertices(GLfloat rounds, GLfloat height, GLfloat thickness, GLfloat radius)
{
    const int slices = 8;
    const int step = 5;

    vertices.clear();
    tex_coords.clear();
    indices.clear();

    for (int i = -slices; i <= rounds * 360 + step; i += step)
    {
        for (int j = 0; j < slices; j++)
        {
            GLfloat t = (GLfloat)i / 360.0f + (GLfloat)j / slices * step / 360.0f;
            t = std::max(0.0f, std::min(rounds, t));
            GLfloat t_i = t * (GLfloat)M_PI * 2;
            GLfloat u_i = (GLfloat)j / slices * (GLfloat)M_PI * 2;
            GLfloat d = radius + thickness * (GLfloat)cos(u_i);
            vertices.push_back(d * (GLfloat)cos(t_i));
            vertices.push_back(d * (GLfloat)sin(t_i));
            vertices.push_back(thickness * (GLfloat)sin(u_i) + height * t / rounds);
            tex_coords.push_back(d * (GLfloat)cos(t_i));
            tex_coords.push_back(d * (GLfloat)sin(t_i));
        }
    }
    for (GLuint i = 0; i < (GLuint)vertices.size() / 3 - slices; ++i)
    {
        indices.push_back(i);
        indices.push_back(i + slices);
    }
}

void Spring::draw() const
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices.data());
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, tex_coords.data());
    glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, indices.data());
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}
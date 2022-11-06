#include "Texture.h"

#include <cstdint>
#include <cstdio>
#include <vector>

GLuint LoadTexture(const char *filename)
{
    FILE *file = fopen(filename, "rb");

    unsigned char header[54];
    fread(header, sizeof(*header), sizeof(header), file);

    unsigned dataPos = *(unsigned *)&(header[0x0A]);
    unsigned imageSize = *(unsigned *)&(header[0x22]);
    unsigned width = *(unsigned *)&(header[0x12]);
    unsigned height = *(unsigned *)&(header[0x16]);

    if (imageSize == 0)
    {
        imageSize = width * height * 3;
    }
    if (dataPos == 0)
    {
        dataPos = 54;
    }

    std::vector<uint8_t> data;
    data.resize(imageSize);

    fseek(file, dataPos, SEEK_SET);
    fread((void *)(data.data()), 1, imageSize, file);
    fclose(file);

    glActiveTexture(GL_TEXTURE1);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, (void *)(data.data()));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, NULL);

    return texture;
}
#include "Texture.h"

#include <cstdint>
#include <cstdio>
#include <fstream>
#include <vector>

GLuint LoadTexture(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary);

    char header[54];
    file.read(header, sizeof(header));

    unsigned dataPos = *(unsigned *)&(header[0x0A]);
    unsigned imageSize = *(unsigned *)&(header[0x22]);
    unsigned width = *(unsigned *)&(header[0x12]);
    unsigned height = *(unsigned *)&(header[0x16]);

    if (imageSize == 0 || dataPos == 0)
    {
        throw std::runtime_error("Invalid BMP file header, file: " + filename);
    }

    std::vector<char> data;
    data.resize(imageSize);
    file.read(data.data(), imageSize);

    glActiveTexture(GL_TEXTURE1);

    GLuint texture;
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, reinterpret_cast<void *>(data.data()));
    glGenerateMipmap(GL_TEXTURE_2D);

    return texture;
}
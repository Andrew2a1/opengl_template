#pragma once

#include <string>

class Shader
{
public:
    unsigned int ID;

public:
    Shader(const std::string &vertexPath, const std::string &fragmentPath);

    void use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

private:
    void checkCompileErrors(unsigned int shader, const std::string &type);
};
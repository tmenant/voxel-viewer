#pragma once

#include "glm/ext/matrix_float4x4.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h> // include glad to get the required OpenGL headers

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>

class Shader
{
    GLuint shaderProgram;
    mutable std::unordered_map<std::string, GLint> uniforms;

public:
    Shader(const char *vertexPath, const char *fragmentPath)
    {
        std::string vertexCode = readShaderCode(vertexPath);
        std::string fragmentCode = readShaderCode(fragmentPath);

        GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode.c_str());
        GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        GLint success;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        if (!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            glDeleteProgram(shaderProgram);

            throw std::runtime_error("Shader linking failed");
        }
    }

    ~Shader()
    {
        glDeleteProgram(shaderProgram);
    }

    Shader(const Shader &) = delete;
    Shader &operator=(const Shader &) = delete;

    void use() const
    {
        glUseProgram(shaderProgram);
    }

    void setBool(const std::string &name, bool value) const
    {
        glUniform1i(getUniformLocation(name), (int)value);
    }

    void setInt(const std::string &name, int value) const
    {
        glUniform1i(getUniformLocation(name), value);
    }

    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(getUniformLocation(name), value);
    }

    void setVec4(const std::string &name, const glm::vec4 &color) const
    {
        glUniform4f(getUniformLocation(name), color.r, color.g, color.b, color.a);
    }

    void setMat4(const std::string &name, const glm::mat4 &matrix) const
    {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
    }

    GLint getUniformLocation(const std::string &name) const
    {
        auto it = uniforms.find(name);
        if (it != uniforms.end())
        {
            return it->second;
        }

        return uniforms.emplace(name, glGetUniformLocation(shaderProgram, name.c_str())).first->second;
    }

private:
    std::string readShaderCode(const std::string &shaderPath) const
    {
        std::ifstream vShaderFile;

        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            vShaderFile.open(shaderPath);

            std::stringstream vShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            vShaderFile.close();
            return vShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            throw std::runtime_error("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " + shaderPath);
        }
    }

    GLuint compileShader(GLenum type, const char *source) const
    {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, NULL, infoLog);

            throw std::runtime_error("Shader compilation failed");
        }

        return shader;
    }
};
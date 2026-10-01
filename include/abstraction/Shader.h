#pragma once

#include <glad/glad.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>


class Shader
{
public:
    unsigned int ID;
    /*
    * @para vertexPath: vertex shader source file
    * @para fragmentPath: fragment shader source file
    */
    Shader(const char* vertexPath, const char* fragmentPath) {

        std::string vertexSource;
        std::string fragmentSource;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            vShaderFile.close();
            fShaderFile.close();
            vertexSource = vShaderStream.str();
            fragmentSource = fShaderStream.str();

        } catch(const std::ifstream::failure& e) {
            std::cout << "ERROR::SHADER::FILE NOT SUCCESFULLY READ" << std::endl;
            exit(1);
        }

        const char* vShaderCode = vertexSource.c_str();
        const char* fShaderCode = fragmentSource.c_str();
        unsigned int vertexID, fragmentID;
        int sucess;
        char infoLog[512];

        vertexID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexID, 1, &vShaderCode, NULL);
        glCompileShader(vertexID);
        glGetShaderiv(vertexID, GL_COMPILE_STATUS, &sucess);
        if (!sucess) {
            glGetShaderInfoLog(vertexID, sizeof(infoLog), NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }


        fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentID, 1, &fShaderCode, NULL);
        glCompileShader(fragmentID);
        glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &sucess);
        if (!sucess) {
            glGetShaderInfoLog(fragmentID, sizeof(infoLog), NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }


        this->ID = glCreateProgram();
        glAttachShader(this->ID, vertexID);
        glAttachShader(this->ID, fragmentID);
        glLinkProgram(this->ID);
        glGetProgramiv(this->ID, GL_LINK_STATUS, &sucess);
        if (!sucess) {
            glGetProgramInfoLog(this->ID, sizeof(infoLog), NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        glDeleteShader(vertexID);
        glDeleteShader(fragmentID);
    }

    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath) {

        std::string vertexSource;
        std::string fragmentSource;
        std::string geometrySource;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        std::ifstream gShaderFile;
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            gShaderFile.open(geometryPath);
            std::stringstream vShaderStream, fShaderStream, gShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            gShaderStream << gShaderFile.rdbuf();

            vShaderFile.close();
            fShaderFile.close();
            gShaderFile.close();
            vertexSource = vShaderStream.str();
            fragmentSource = fShaderStream.str();
            geometrySource = gShaderStream.str();

        } catch(const std::ifstream::failure& e) {
            std::cout << "ERROR::SHADER::FILE NOT SUCCESFULLY READ" << std::endl;
            exit(1);
        }

        const char* vShaderCode = vertexSource.c_str();
        const char* fShaderCode = fragmentSource.c_str();
        const char* gShaderCode = geometrySource.c_str();
        unsigned int vertexID, fragmentID, geometryID;
        int sucess;
        char infoLog[512];

        vertexID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexID, 1, &vShaderCode, NULL);
        glCompileShader(vertexID);
        glGetShaderiv(vertexID, GL_COMPILE_STATUS, &sucess);
        if (!sucess) {
            glGetShaderInfoLog(vertexID, sizeof(infoLog), NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }


        fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentID, 1, &fShaderCode, NULL);
        glCompileShader(fragmentID);
        glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &sucess);
        if (!sucess) {
            glGetShaderInfoLog(fragmentID, sizeof(infoLog), NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        geometryID = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryID, 1, &gShaderCode, NULL);
        glCompileShader(geometryID);
        glGetShaderiv(geometryID, GL_COMPILE_STATUS, &sucess);
        if (!sucess) {
            glGetShaderInfoLog(geometryID, sizeof(infoLog), NULL, infoLog);
            std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        this->ID = glCreateProgram();
        glAttachShader(this->ID, vertexID);
        glAttachShader(this->ID, fragmentID);
        glAttachShader(this->ID, geometryID);
        glLinkProgram(this->ID);
        glGetProgramiv(this->ID, GL_LINK_STATUS, &sucess);
        if (!sucess) {
            glGetProgramInfoLog(this->ID, sizeof(infoLog), NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        glDeleteShader(vertexID);
        glDeleteShader(fragmentID);
        glDeleteShader(geometryID);
    }
    inline void use() {
        glUseProgram(this->ID);
    }
    // void setUniform_Bool (const std::string& name,  bool value) const;
    // void setUniform_Int  (const std::string& name,   int value) const;
    // void setUniform_Float(const std::string& name, float value) const;

    void setUniform(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
    }
    void setUniform(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
    }
    void setUniform(const std::string& name, bool value) const {
        glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)value);
    }

    void setMat4(const std::string& name, const float* mat4) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, mat4);
    }

    void setVec2(const std::string& name, const float* vec2) const {
        glUniform2fv(glGetUniformLocation(this->ID, name.c_str()), 1, vec2);
    }
};

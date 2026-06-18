#include "Shader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Engine {

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
    : m_vertexPath(vertexPath), m_fragmentPath(fragmentPath), m_programID(0) {}

Shader::~Shader() {
    if (m_programID != 0) {
        glDeleteProgram(m_programID);
    }
}

bool Shader::Load() {
    std::string vertexCode = ReadFile(m_vertexPath);
    std::string fragmentCode = ReadFile(m_fragmentPath);

    if (vertexCode.empty() || fragmentCode.empty()) {
        std::cerr << "[Engine::Shader] Failed to load shader files: " 
                  << m_vertexPath << " or " << m_fragmentPath << std::endl;
        return false;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;

    // Compile Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    if (!CheckCompileErrors(vertex, "VERTEX")) {
        return false;
    }

    // Compile Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    if (!CheckCompileErrors(fragment, "FRAGMENT")) {
        return false;
    }

    // Create Shader Program
    m_programID = glCreateProgram();
    glAttachShader(m_programID, vertex);
    glAttachShader(m_programID, fragment);
    glLinkProgram(m_programID);
    if (!CheckCompileErrors(m_programID, "PROGRAM")) {
        return false;
    }

    // Delete shaders as they are linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return true;
}

void Shader::Use() {
    glUseProgram(m_programID);
}

void Shader::Unuse() {
    glUseProgram(0);
}

void Shader::SetBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(m_programID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(m_programID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(m_programID, name.c_str()), value);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(m_programID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(m_programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

std::string Shader::ReadFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "[Engine::Shader] Could not open file: " << filepath << std::endl;
        return "";
    }
    std::stringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

bool Shader::CheckCompileErrors(unsigned int shader, const std::string& type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "[Engine::Shader] ERROR: Shader compilation of type: " << type << "\n"
                      << infoLog << "\n-------------------------------------------------" << std::endl;
            return false;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "[Engine::Shader] ERROR: Program linking\n"
                      << infoLog << "\n-------------------------------------------------" << std::endl;
            return false;
        }
    }
    return true;
}

} // namespace Engine

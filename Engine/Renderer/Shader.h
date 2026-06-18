#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Engine {

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    bool Load();
    void Use();
    void Unuse();

    // Uniform setters
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec3(const std::string& name, const glm::vec3& value) const;
    void SetMat4(const std::string& name, const glm::mat4& mat) const;

    unsigned int GetProgramID() const { return m_programID; }

private:
    std::string ReadFile(const std::string& filepath);
    bool CheckCompileErrors(unsigned int shader, const std::string& type);

    std::string m_vertexPath;
    std::string m_fragmentPath;
    unsigned int m_programID;
};

} // namespace Engine

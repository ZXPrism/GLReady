#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace GLReady {

    struct ShaderInfo
    {
        std::string _vertexShaderPath;
        std::string _tesselControlShaderPath;
        std::string _tesselEvalShaderPath;
        std::string _geometryShaderPath;
        std::string _fragmentShaderPath;
        std::string _computeShaderPath;
    };

    class Shader
    {
    public:
        Shader(const ShaderInfo &shaderInfo);
        ~Shader();

        void Activate();

        void SetUniform(const std::string &name, int value);
        void SetUniform(const std::string &name, float value);
        void SetUniform(const std::string &name, const glm::mat4 &matrix);
        void SetUniform(const std::string &name, const glm::vec3 &vec);

    private:
        int GetUniformLoc(const std::string &name);

    private:
        unsigned _programID;
        std::unordered_map<std::string, int> _uniformMap;
    };

} // namespace GLReady

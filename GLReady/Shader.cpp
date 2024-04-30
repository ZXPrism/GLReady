#include "Shader.h"

#include <fstream>
#include <iostream>
#include <optional>
#include <regex>
#include <sstream>

#include <glad/glad.h>

namespace GLReady {

    Shader::Shader(const ShaderInfo &initStruct)
    {
        static char infoLog[1024];
        int success = 1;

        _programID = glCreateProgram();

        auto LoadShader = [&](const std::string &path, int shaderType) {
            std::optional<GLuint> shader;

            if (path.empty())
                return shader;

            std::string shaderSrc;
            const char *shaderSrcPtr;
            std::ifstream fin(path);
            std::stringstream ssm;

            if (!fin)
            {
                std::cout << "[GLReady / Shader::Shader] fatal: Unable to open file " << path
                          << std::endl;
                return shader;
            }

            ssm << fin.rdbuf();
            shaderSrc = ssm.str();
            fin.close();

            std::regex pattern("uniform\\s+\\S+\\d?\\s+(\\S+);");
            std::smatch match;
            auto targetIter = shaderSrc.cbegin();
            while (std::regex_search(targetIter, shaderSrc.cend(), match, pattern))
            {
                targetIter = match[0].second;
                _uniformMap[match[1].str()] = 0;
                std::cout << "[GLReady / Shader::Shader] info: Detected uniform variable "
                          << match[1].str() << std::endl;
            }

            shaderSrcPtr = shaderSrc.c_str();
            shader = glCreateShader(shaderType);
            glShaderSource(shader.value(), 1, &shaderSrcPtr, nullptr);

            glCompileShader(shader.value());
            glGetShaderiv(shader.value(), GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader.value(), 1024, NULL, infoLog);
                std::cout << infoLog << std::endl;
            }

            glAttachShader(_programID, shader.value());

            return shader;
        };

        std::vector<std::optional<GLuint>> shaders;

        shaders.push_back(LoadShader(initStruct._vertexShaderPath, GL_VERTEX_SHADER));
        shaders.push_back(LoadShader(initStruct._fragmentShaderPath, GL_FRAGMENT_SHADER));
        shaders.push_back(LoadShader(initStruct._tesselControlShaderPath, GL_TESS_CONTROL_SHADER));
        shaders.push_back(LoadShader(initStruct._tesselEvalShaderPath, GL_TESS_EVALUATION_SHADER));
        shaders.push_back(LoadShader(initStruct._geometryShaderPath, GL_GEOMETRY_SHADER));
        shaders.push_back(LoadShader(initStruct._computeShaderPath, GL_COMPUTE_SHADER));

        glLinkProgram(_programID);
        glGetProgramiv(_programID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(_programID, 1024, NULL, infoLog);
            std::cout << infoLog << std::endl;
        }

        for (const auto &shader : shaders)
        {
            if (shader.has_value())
                glDeleteShader(shader.value());
        }

        for (auto &[name, loc] : _uniformMap)
            loc = glGetUniformLocation(_programID, name.c_str());
    }

    Shader::~Shader()
    {
        glDeleteProgram(_programID);
    }

    void Shader::Activate()
    {
        glUseProgram(_programID);
    }

    void Shader::SetUniform(const std::string &name, int value)
    {
        glUniform1i(GetUniformLoc(name), value);
    }

    void Shader::SetUniform(const std::string &name, float value)
    {
        glUniform1f(GetUniformLoc(name), value);
    }

    void Shader::SetUniform(const std::string &name, const glm::mat4 &matrix)
    {
        glUniformMatrix4fv(GetUniformLoc(name), 1, GL_FALSE, &matrix[0][0]);
    }

    void Shader::SetUniform(const std::string &name, const glm::vec3 &vec)
    {
        glUniform3fv(GetUniformLoc(name), 1, &vec[0]);
    }

    int Shader::GetUniformLoc(const std::string &name)
    {
        auto iter = _uniformMap.find(name);
        if (iter == _uniformMap.end())
        {
            std::cout << "[GLReady / Shader::GetUniformLoc] fatal: Unknown uniform name " << name
                      << std::endl;
            return INT_MAX;
        }
        return iter->second;
    }

} // namespace GLReady

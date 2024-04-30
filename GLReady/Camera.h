#pragma once

#include <glm/glm.hpp>

namespace GLReady {

    class Camera
    {
    public:
        const glm::mat4 &GetViewMatrix() const;
        const glm::mat4 &GetProjectionMatrix() const;

        void Update();

        void SetCameraPos(const glm::vec3 &newPos);

        void LookAt(const glm::vec3 &center);
        void Perspective(float fov, float aspectRatio, float zNear, float zFar);

    private:
        glm::mat4 _viewMat = glm::mat4(1.0f);
        glm::mat4 _projMat = glm::mat4(1.0f);

        glm::vec3 _pos = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 _front;
        glm::vec3 _worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    };

} // namespace GLReady

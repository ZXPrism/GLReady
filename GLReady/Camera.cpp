#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace GLReady {

    void Camera::Perspective(float fov, float aspectRatio, float zNear, float zFar)
    {
        _projMat = glm::perspective(fov, aspectRatio, zNear, zFar);
    }

    const glm::mat4 &Camera::GetViewMatrix() const
    {
        return _viewMat;
    }

    const glm::mat4 &Camera::GetProjectionMatrix() const
    {
        return _projMat;
    }

    void Camera::SetCameraPos(const glm::vec3 &newPos)
    {
        _pos = newPos;
    }

    void Camera::LookAt(const glm::vec3 &center)
    {
        _front = glm::normalize(center - _pos);
    }

    void Camera::Update()
    {
        _viewMat = glm::lookAt(_pos, _pos + _front, _worldUp);
    }

} // namespace GLReady

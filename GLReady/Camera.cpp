#include "Camera.h"

#include "Context.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

namespace GLReady {

    Camera::Camera()
    {
        gContext.RegisterOnCursorPosFunc([&](double xPos, double yPos) {
            static double xpos, ypos;

            if (glfwGetMouseButton(gContext.GetWindowHandle(), GLFW_MOUSE_BUTTON_LEFT) ==
                GLFW_PRESS)
            { // mainly from LearnOpenGL
                double dx = xPos - xpos, dy = ypos - yPos;

                dx *= _mouseSensitivity;
                dy *= _mouseSensitivity;

                _yaw += dx;
                _pitch += dy;

                if (_pitch > 89.0f)
                    _pitch = 89.0f;
                if (_pitch < -89.0f)
                    _pitch = -89.0f;

                glm::vec3 front;
                front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
                front.y = sin(glm::radians(_pitch));
                front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
                _front = glm::normalize(front);

                xpos = xPos;
                ypos = yPos;
            }

            xpos = xPos;
            ypos = yPos;
        });
    }

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

    void Camera::Update(float dt)
    {
        auto window = gContext.GetWindowHandle();
        float speed = _speed * dt;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            _pos += speed * _front;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            _pos -= speed * _front;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            _pos -= glm::normalize(glm::cross(_front, _worldUp)) * speed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            _pos += glm::normalize(glm::cross(_front, _worldUp)) * speed;

        _viewMat = glm::lookAt(_pos, _pos + _front, _worldUp);
    }

} // namespace GLReady

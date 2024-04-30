#pragma once

#include <functional>
#include <string>

struct GLFWwindow;

namespace GLReady {

    class Context
    {
    public:
        void Init(const std::string &windowTitle, int windowWidth, int windowHeight);
        void Run(const std::function<void(float)> &callback);
        void Shutdown();

        void SetClearColor(float r, float g, float b);
        void SetClearColor(int r, int g, int b);
        void SetClearColor(int rgb);

        void EnableVSync();
        void EnableDepthTest();

        GLFWwindow *GetWindowHandle();

    private:
        void Callback_WindowSize(int width, int height);

    private:
        GLFWwindow *_windowHandle;
        int _clearMask = 0;
    };

} // namespace GLReady

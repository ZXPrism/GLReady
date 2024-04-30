#pragma once

#include <functional>
#include <string>
#include <vector>

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

        // callbacks
        using OnCursorPosFunc = std::function<void(double, double)>;
        using OnKeyFunc = std::function<void(int, int, int, int)>;
        using OnMouseButtonFunc = std::function<void(int, int, int)>;
        using OnScrollFunc = std::function<void(double, double)>;
        using OnWindowSizeFunc = std::function<void(int, int)>;

        void RegisterOnKeyFunc(OnKeyFunc func);
        void RegisterOnMouseButtonFunc(OnMouseButtonFunc func);
        void RegisterOnCursorPosFunc(OnCursorPosFunc func);
        void RegisterOnScrollFunc(OnScrollFunc func);
        void RegisterOnWindowSizeFunc(OnWindowSizeFunc func);

        void OnKey(int key, int scanCode, int action, int mods);
        void OnMouseButton(int button, int action, int mods);
        void OnCursorPos(double xPos, double yPos);
        void OnScroll(double xOffset, double yOffset);
        void OnWindowSize(int width, int height);

    private:
        void InitGLFW(const std::string &windowTitle, int windowWidth, int windowHeight);
        void InitGLAD(int windowWidth, int windowHeight);
        void InitImGui();
        void InitCallbacks();

    private:
        GLFWwindow *_windowHandle;
        int _clearMask = 0;

        std::vector<OnCursorPosFunc> _onCursorPosVec;
        std::vector<OnKeyFunc> _onKeyVec;
        std::vector<OnMouseButtonFunc> _onMouseButtonVec;
        std::vector<OnScrollFunc> _onScrollVec;
        std::vector<OnWindowSizeFunc> _onWindowSizeVec;
    };

} // namespace GLReady

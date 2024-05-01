#include "Context.h"

#include <chrono>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

namespace GLReady {

    Context gContext;

    void Context::Init(const std::string &windowTitle, int windowWidth, int windowHeight)
    {
        _windowWidth = windowWidth;
        _windowHeight = windowHeight;

        InitGLFW(windowTitle);
        InitGLAD();
        InitCallbacks();
        InitImGui();

        RegisterOnWindowSizeFunc([&](int width, int height) {
            glViewport(0, 0, width, height);

            _windowWidth = width;
            _windowHeight = height;
        });
    }

    void Context::InitGLFW(const std::string &windowTitle)
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);
        _windowHandle =
            glfwCreateWindow(_windowWidth, _windowHeight, windowTitle.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(_windowHandle);

        auto vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowPos(_windowHandle, (vidmode->width - _windowWidth) / 2,
                         (vidmode->height - _windowHeight) / 2);
    }

    void Context::InitGLAD()
    {
        gladLoadGL();
        glEnable(GL_MULTISAMPLE);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glViewport(0, 0, _windowWidth, _windowHeight);
        _clearMask |= GL_COLOR_BUFFER_BIT;
    }

    void Context::InitImGui()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(_windowHandle, true);
        ImGui_ImplOpenGL3_Init("#version 450");
        ImGui::StyleColorsDark();

        auto &style = ImGui::GetStyle();
        auto &colors = style.Colors;

        colors[ImGuiCol_TitleBg] = {0.113f, 0.113f, 0.113f, 1.0f};
        colors[ImGuiCol_TitleBgActive] = {0.187f, 0.187f, 0.187f, 1.0f};

        colors[ImGuiCol_Tab] = {0.113f, 0.113f, 0.113f, 1.0f};
        colors[ImGuiCol_TabUnfocusedActive] = {0.113f, 0.113f, 0.113f, 1.0f};
        colors[ImGuiCol_TabActive] = {0.5f, 0.5f, 0.5f, 1.0f};
        colors[ImGuiCol_TabHovered] = {0.3f, 0.3f, 0.3f, 1.0f};

        colors[ImGuiCol_HeaderHovered] = {0.3f, 0.3f, 0.3f, 1.0f};

        colors[ImGuiCol_FrameBg] = {0.3f, 0.3f, 0.3f, 1.0f};

        style.WindowPadding = {8, 10};
        style.FramePadding = {8, 8};
        style.ItemSpacing = {8, 8};
        style.ItemInnerSpacing = {8, 8};
        style.ScrollbarSize = 16;
        style.FrameRounding = 4;
        style.WindowRounding = 5;
    }

    void Context::InitCallbacks()
    {
        glfwSetWindowUserPointer(_windowHandle, this);

        glfwSetCursorPosCallback(_windowHandle, [](GLFWwindow *window, double xPos, double yPos) {
            ((Context *)glfwGetWindowUserPointer(window))->OnCursorPos(xPos, yPos);
        });
        glfwSetKeyCallback(
            _windowHandle, [](GLFWwindow *window, int key, int scanCode, int action, int mods) {
                ((Context *)glfwGetWindowUserPointer(window))->OnKey(key, scanCode, action, mods);
            });
        glfwSetMouseButtonCallback(
            _windowHandle, [](GLFWwindow *window, int button, int action, int mods) {
                ((Context *)glfwGetWindowUserPointer(window))->OnMouseButton(button, action, mods);
            });
        glfwSetScrollCallback(
            _windowHandle, [](GLFWwindow *window, double xOffset, double yOffset) {
                ((Context *)glfwGetWindowUserPointer(window))->OnScroll(xOffset, yOffset);
            });
        glfwSetWindowSizeCallback(_windowHandle, [](GLFWwindow *window, int width, int height) {
            ((Context *)glfwGetWindowUserPointer(window))->OnWindowSize(width, height);
        });
    }

    void Context::Run(const std::function<void(float)> &callback)
    {
        namespace ch = std::chrono;
        auto t1 = ch::steady_clock::now();
        float dt = 0.0f;
        while (!glfwWindowShouldClose(_windowHandle))
        {
            glClear(_clearMask);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            callback(dt);

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwPollEvents();
            glfwSwapBuffers(_windowHandle);

            auto t2 = ch::steady_clock::now();
            dt = ch::duration_cast<ch::microseconds>(t2 - t1).count() / 1000000.0;
            t1 = t2;
        }
    }

    void Context::Shutdown()
    {
        ImGui_ImplGlfw_Shutdown();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(_windowHandle);
        glfwTerminate();
    }

    void Context::EnableVSync()
    {
        glfwSwapInterval(1);
    }

    void Context::EnableDepthTest()
    {
        glEnable(GL_DEPTH_TEST);
        _clearMask |= GL_DEPTH_BUFFER_BIT;
    }

    GLFWwindow *Context::GetWindowHandle()
    {
        return _windowHandle;
    }

    int Context::GetWindowWidth() const
    {
        return _windowWidth;
    }

    int Context::GetWindowHeight() const
    {
        return _windowHeight;
    }

    void Context::SetClearColor(float r, float g, float b)
    {
        glClearColor(r, g, b, 1.0f);
    }

    void Context::SetClearColor(int r, int g, int b)
    {
        glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
    }

    void Context::SetClearColor(int rgb)
    {
        int r = rgb >> 16, g = (rgb >> 8) & 0xff, b = rgb & 0xff;
        glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
    }

    void Context::RegisterOnCursorPosFunc(OnCursorPosFunc func)
    {
        _onCursorPosVec.push_back(func);
    }

    void Context::RegisterOnKeyFunc(OnKeyFunc func)
    {
        _onKeyVec.push_back(func);
    }

    void Context::RegisterOnMouseButtonFunc(OnMouseButtonFunc func)
    {
        _onMouseButtonVec.push_back(func);
    }

    void Context::RegisterOnScrollFunc(OnScrollFunc func)
    {
        _onScrollVec.push_back(func);
    }

    void Context::RegisterOnWindowSizeFunc(OnWindowSizeFunc func)
    {
        _onWindowSizeVec.push_back(func);
    }

    void Context::OnCursorPos(double xPos, double yPos)
    {
        for (auto &fn : _onCursorPosVec)
            fn(xPos, yPos);
    }

    void Context::OnKey(int key, int scanCode, int action, int mods)
    {
        for (auto &fn : _onKeyVec)
            fn(key, scanCode, action, mods);
    }

    void Context::OnMouseButton(int button, int action, int mods)
    {
        for (auto &fn : _onMouseButtonVec)
            fn(button, action, mods);
        ;
    }

    void Context::OnScroll(double xOffset, double yOffset)
    {
        for (auto &fn : _onScrollVec)
            fn(xOffset, yOffset);
    }

    void Context::OnWindowSize(int width, int height)
    {
        for (auto &fn : _onWindowSizeVec)
            fn(width, height);
    }

} // namespace GLReady

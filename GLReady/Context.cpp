#include "Context.h"

#include <chrono>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

namespace GLReady {

    void Context::Init(const std::string &windowTitle, int windowWidth, int windowHeight)
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);
        _windowHandle =
            glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(_windowHandle);
        glfwSetWindowUserPointer(_windowHandle, this);
        glfwSetWindowSizeCallback(_windowHandle, [](GLFWwindow *window, int width, int height) {
            static_cast<Context *>(glfwGetWindowUserPointer(window))
                ->Callback_WindowSize(width, height);
        });

        gladLoadGL();
        glEnable(GL_MULTISAMPLE);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glViewport(0, 0, windowWidth, windowHeight);
        _clearMask |= GL_COLOR_BUFFER_BIT;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(_windowHandle, true);
        ImGui_ImplOpenGL3_Init("#version 450");
        ImGui::StyleColorsDark();
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

    void Context::Callback_WindowSize(int width, int height)
    {
        glViewport(0, 0, width, height);
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

} // namespace GLReady

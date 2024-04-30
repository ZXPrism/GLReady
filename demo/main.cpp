#include "Context.h"
#include "Shader.h"
#include "VertexBuffer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

#include <ctime>

int main(int argc, char *argv[])
{
    using namespace GLReady;

    Context context;

    context.Init("GLReady Demo", 1200, 960);
    context.EnableVSync();
    context.EnableDepthTest();
    context.SetClearColor(0xffffff);

    float vertices[] = {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
                        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
                        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

                        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
                        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                        -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

                        -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
                        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

                        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
                        0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
                        0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
                        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
                        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

                        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
                        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                        -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};
    VertexBuffer cube;
    cube.SetData(&vertices[0], sizeof(vertices) / sizeof(float))
        .SetUsage(0, 3)
        .SetUsage(1, 2)
        .EndSetUsage();

    ShaderInfo shaderInit;
    shaderInit._vertexShaderPath = "shaders/vertex.vs";
    shaderInit._fragmentShaderPath = "shaders/frag.fs";
    Shader shader(shaderInit);

    glm::mat4 view(1.0f), proj(1.0f);
    view = glm::lookAt(glm::vec3{2.0f, 2.0f, 2.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
    proj = glm::perspective(45.0f, 1.0f * 1200 / 960, 0.1f, 100.0f);

    shader.Activate();
    shader.SetUniform("view", view);
    shader.SetUniform("proj", proj);

    context.Run([&](float dt) {
        glm::mat4 model(1.0f);
        model = glm::rotate(model, glm::radians(90 * dt), {1.0f, 0.0f, 0.0f});
        model = glm::rotate(model, glm::radians(45 * dt), {0.0f, 1.0f, 0.0f});
        model = glm::rotate(model, glm::radians(25 * dt), {0.0f, 0.0f, 1.0f});

        shader.SetUniform("model", model);
        shader.Activate();

        glm::vec3 a(0.5f, 1.0f, 0.0f), b(0.0f, 0.5f, 1.0f);
        float t = 0.0f;
        for (int i = 0; i < 6; i++)
        {
            shader.SetUniform("color", a * (1 - t) + b * t);
            t += 1 / 6.0f;

            cube.DrawTriangles(6 * i, 6);
        }
    });

    context.Shutdown();

    return 0;
}

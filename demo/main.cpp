#include "Camera.h"
#include "Context.h"
#include "Shader.h"
#include "UI.h"
#include "VertexBuffer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

#include <ctime>

int main(int argc, char *argv[])
{
    using namespace GLReady;

    gContext.Init("GLReady Demo", 1280, 960);
    gContext.EnableVSync();
    gContext.EnableDepthTest();
    gContext.SetClearColor(0xffffff);

    ui::SetFont("c:/windows/fonts/consola.ttf", 20);

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

    Camera camera;
    camera.Perspective(45.0f, 1.0f * 1200 / 960, 0.1f, 100.0f);
    camera.SetCameraPos({2.0f, 2.0f, 5.0f});

    shader.Activate();
    shader.SetUniform("proj", camera.GetProjectionMatrix());

    glm::vec3 scale(1.0f, 1.0f, 1.0f);
    glm::vec3 a(0.5f, 1.0f, 0.0f), b(0.0f, 0.5f, 1.0f);

    gContext.Run([&](float dt) {
        camera.Update(dt);
        shader.SetUniform("view", camera.GetViewMatrix());

        glm::mat4 model(1.0f);
        model = glm::scale(model, scale);

        shader.Activate();

        for (int x = -1; x <= 1; x++)
        {
            for (int y = -1; y <= 1; y++)
            {
                for (int z = -1; z <= 1; z++)
                {
                    float t = 0.0f;
                    for (int i = 0; i < 6; i++)
                    {
                        glm::vec3 pos(x, y, z);

                        shader.SetUniform("model", glm::translate(model, pos));
                        shader.SetUniform("color", a * (1 - t) + b * t);
                        t += 1 / 6.0f;

                        cube.DrawTriangles(6 * i, 6);
                    }
                }
            }
        }

        ImGui::Begin("object property");
        ImGui::SeparatorText("Transform");
        ImGui::InputFloat3("scale", &scale.x);
        ImGui::SeparatorText("Shading");
        ImGui::ColorEdit3("Interp Color A", &a.x);
        ImGui::ColorEdit3("Interp Color B", &b.x);
        ImGui::End();
    });

    gContext.Shutdown();

    return 0;
}

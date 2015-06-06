#include <GLFW/glfw3.h>

#include <imgui.h>
#include "imgui_impl_glfw.h"
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "context.h"
#include "core.h"
#include "entities.h"
#include "game.h"
#include "renderqueue.h"
#include "shaders.h"

using namespace daisy;

static void errorCallback(int error, const char* description)
{
    fputs(description, stderr);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

static void framebufferSizeCallback(GLFWwindow* window, int w, int h)
{
    Screen::ViewportChanged = true;
}

int main()
{
    Log("...daisy...\n");

    glfwSetErrorCallback(errorCallback);

    if (!glfwInit())
        exit(EXIT_FAILURE);
    
    auto window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetKeyCallback(window, keyCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    ImGui_ImplGlfw_Init(window, true);

    Screen screen(window);
    daisy::Shaders shaders;
    RenderQueue rq;
    Context ctx(screen, shaders, rq);

    Entities ents;

    LoadGame(ctx, ents);

    bool show_test_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImColor(114, 144, 154);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Imgui test
        ImGui_ImplGlfw_NewFrame();

        // 1. Show a simple window
        // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
        /*{
            static float f = 0.0f;
            ImGui::Text("Hello, world!");
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            ImGui::ColorEdit3("clear color", (float*)&clear_color);
            if (ImGui::Button("Test Window")) show_test_window ^= 1;
            if (ImGui::Button("Another Window")) show_another_window ^= 1;
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        // 2. Show another simple window, this time using an explicit Begin/End pair
        if (show_another_window)
        {
            ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiSetCond_FirstUseEver);
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello");
            ImGui::End();
        }

        // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
        if (show_test_window)
        {
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
            ImGui::ShowTestWindow(&show_test_window);
        }*/

        // Prepare context for new frame
        ctx.NewFrame();

        ctx.RenderQueue().SetOrthoProj(ctx.Screen());

        // Update components of all entities
        ents.Update();

        glm::mat4 tx;
        tx = glm::rotate(tx, (float)ctx.TotalElapsedTime() * 50.f, glm::vec3(0.f, 0.f, 1.f));
        ctx.RenderQueue().AddMesh(tx);

        ctx.RenderQueue().Render();

        glBegin(GL_TRIANGLES);
        glColor3f(1.f, 0.f, 0.f);
        glVertex3f(-0.6f, -0.4f, 0.f);
        glColor3f(0.f, 1.f, 0.f);
        glVertex3f(0.6f, -0.4f, 0.f);
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.f, 0.6f, 0.f);
        glEnd();

        ImGui::Render();

        glfwSwapBuffers(window);
    }

    ImGui_ImplGlfw_Shutdown();

    glfwDestroyWindow(window);
    glfwTerminate();

    ImGui::LogFinish();
    
    exit(EXIT_SUCCESS);
}

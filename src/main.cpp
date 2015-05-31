#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

#include "context.h"
#include "entities.h"
#include "game.h"
#include "renderqueue.h"

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

    Screen screen(window);
    Context ctx(screen);

    Entities ents;
    RenderQueue rq;

    LoadGame(ents);

    while (!glfwWindowShouldClose(window))
    {
        // Prepare context for new frame
        ctx.NewFrame();

        // Empty queue
        rq.Clear();

        // Update components of all entities
        ents.Update();

        // Render queue
        rq.Render();

        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ctx.Screen().AspectRatio(), ctx.Screen().AspectRatio(), -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef((float)ctx.TotalElapsedTime() * 50.f, 0.f, 0.f, 1.f);

        glBegin(GL_TRIANGLES);
        glColor3f(1.f, 0.f, 0.f);
        glVertex3f(-0.6f, -0.4f, 0.f);
        glColor3f(0.f, 1.f, 0.f);
        glVertex3f(0.6f, -0.4f, 0.f);
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.f, 0.6f, 0.f);
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    
    exit(EXIT_SUCCESS);
}

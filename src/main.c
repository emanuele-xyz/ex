#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Ex/Core.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    ExUnused(window);

    glViewport(0, 0, width, height);
}

int main(void)
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Ex Game", NULL, NULL);
    if (window == NULL)
    {
        // TODO: oh no!
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);


    if (!(gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress))))
    {
        // TODO: oh no!
    }

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}

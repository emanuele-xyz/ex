#include <stdio.h>
#include <GLFW/glfw3.h>

int main(void)
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Ex Game", NULL, NULL);
    if (window == NULL)
    {
        // TODO: oh no!
    }

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

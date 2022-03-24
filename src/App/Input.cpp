#include "Ex/App/Input.h"

#pragma warning (push, 0)
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#pragma warning (pop)

#include "Ex/Systems/Memory.h"

void Input::Init(void* windowHandle)
{
    glfwSetWindowUserPointer((GLFWwindow*)(windowHandle), this);

    glfwSetFramebufferSizeCallback((GLFWwindow*)(windowHandle), (GLFWframebuffersizefun)(FramebufferSizeCallback));
    glfwSetWindowCloseCallback((GLFWwindow*)(windowHandle), (GLFWwindowclosefun)(CloseCallback));

    // TODO: handle a variable amount of events
    nextEventIndex = 0;
    eventCount = 1024;
    events = ExArrayAlloc<InputEvent>(eventCount);
}

void Input::Fini()
{
    ExArrayFree(events);
    eventCount = 0;
    nextEventIndex = 0;
}

u32 Input::GetEventCount()
{
    return nextEventIndex;
}

InputEvent* Input::GetEvents()
{
    return events;
}

void Input::ClearEvents()
{
    for (u32 i = 0; i < nextEventIndex; i++)
    {
        events[i] = {};
    }

    nextEventIndex = 0;
}

void Input::PostEvent(InputEvent event)
{
    ExAssert(nextEventIndex < eventCount);
    events[nextEventIndex++] = event;
}

void Input::FramebufferSizeCallback(void* window, i32 width, i32 height)
{
    Input* input = (Input*)(glfwGetWindowUserPointer((GLFWwindow*)(window)));

    InputEvent event = {};
    event.type = InputEventType::FramebufferSize;
    event.framebufferSize = { width, height };
    input->PostEvent(event);
}

void Input::CloseCallback(void* window)
{
    Input* input = (Input*)(glfwGetWindowUserPointer((GLFWwindow*)(window)));

    InputEvent event = {};
    event.type = InputEventType::Close;
    input->PostEvent(event);
}

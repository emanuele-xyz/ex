#include "Ex/Game/Game.h"

#include "Ex/Systems/Memory.h"
#include "Ex/Systems/Logger.h"

void Game::Init(void* windowHandle)
{
    graphics.Init(windowHandle);
    hasFinished = false;

    // -------------------------------------------------------------------------


}

void Game::Fini()
{


    // -------------------------------------------------------------------------

    hasFinished = true;
    graphics.Fini();
}

void Game::ProcessInput(u32 inputEventCount, InputEvent* inputEvents)
{
    for (u32 i = 0; i < inputEventCount; i++)
    {
        switch (inputEvents[i].type)
        {
            case InputEventType::None:
            {
                /* NOTE: do nothing */
            } break;

            case InputEventType::FramebufferSize:
            {
                i32 width = inputEvents[i].framebufferSize.width;
                i32 height = inputEvents[i].framebufferSize.height;
                graphics.ResizeFramebuffer(width, height);
            } break;

            case InputEventType::Close:
            {
                hasFinished = true;
            } break;

            default:
            {
                ExUnreachableCodePath();
            } break;
        }
    }
}

bool Game::Update()
{
    graphics.ClearColorBuffer(0.2f, 0.3f, 0.3f, 1.0f);

    return hasFinished;
}

#include "Ex/Engine/Modules/Game.h"

#include "Ex/Systems/Memory.h"
#include "Ex/Systems/Logger.h"

// -----------------------------------------------------------------------------
// TODO: move these in their own files
// -----------------------------------------------------------------------------

#pragma warning (push, 0)
#include <cglm/cglm.h>
#pragma warning (pop)

typedef u32 Mesh;
typedef u32 Material;

class TransformComponent
{
    vec3 position;
    vec3 rotation;
};

class MeshComponent
{
    Mesh mesh;
}

class MaterialComponent
{
    Material material;
}

class LocalSystem
{

};

class Entity
{
public:
    virtual void Update() {};
};

class Player : public Entity
{
    TransformComponent transform;
}

class GlobalSystem
{

};

// -----------------------------------------------------------------------------

void GameModule::Init()
{
    // TODO: use a Vec
    listenersCount = 16;
    listeners = ExArrayAlloc<GameModuleEventsListener*>(listenersCount);
}

void GameModule::Fini()
{
    ExFree(listeners);
    listenersCount = 0;
}

void GameModule::Tick()
{

}

void GameModule::AddListener(GameModuleEventsListener* listener)
{
    // TODO: to be reworked when using Vec

    u32 i = 0;
    while (i < listenersCount && listeners[i] != nullptr) i++;
    ExAssert(i != listenersCount);

    listeners[i] = listener;
}

void GameModule::EmitQuitEvent()
{
    for (u32 i = 0; i < listenersCount; i++)
    {
        if (listeners[i] != nullptr)
        {
            listeners[i]->OnQuit();
        }
    }
}

void GameModule::OnCloseRequest()
{
    EmitQuitEvent();
}

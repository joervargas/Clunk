#include "SandBox.h"


Clunk::Application* Clunk::CreateApplication()
{
    return new Sandbox();
}

Sandbox::~Sandbox()
{
    delete(OnEscPressed);
}

b8 Sandbox::Initialize()
{
    b8 bInitialized = Application::Initialize();

    // #ifdef VULKAN_GFX
    //     Clunk::ClVkContext* ctx = Clunk::ClVkContext::Get();
    // #endif

    OnEscPressed = new KeyListener<Sandbox>(EKeyState::KS_PRESSED, &KeyEvents::KE_ESC, this, &Application::Escape);
    return bInitialized;
}

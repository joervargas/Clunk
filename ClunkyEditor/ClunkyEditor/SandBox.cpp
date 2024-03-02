#include "SandBox.h"


Clunk::Application* Clunk::CreateApplication()
{
    return new Sandbox();
}

Sandbox::~Sandbox()
{
    CLUNK_DELETE(EscKeyListener);
}

b8 Sandbox::Initialize()
{
    b8 bInitialized = Application::Initialize();

    EscKeyListener = new Clunk::Events::ButtonListener<Sandbox>(&Clunk::Events::Keys::ESC_Key, this, &Application::Escape);

    return bInitialized;
}
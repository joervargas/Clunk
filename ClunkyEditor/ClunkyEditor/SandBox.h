#include <ClunkEngine/Engine.h>


class Sandbox : public Clunk::Application
{
public:

    Sandbox(){};
    ~Sandbox();

    virtual b8 Initialize() override;

private:

    KeyListener<Sandbox>* OnEscPressed;

};




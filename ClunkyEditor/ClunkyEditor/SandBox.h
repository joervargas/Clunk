#include <ClunkEngine/Engine.h>


class Sandbox : public Clunk::Application
{
public:

    Sandbox(){};
    ~Sandbox();

    virtual b8 Initialize() override;

private:

    Clunk::Events::ButtonListener<Sandbox>* EscKeyListener;

};




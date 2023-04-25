#include "ClLayerStack.h"

namespace Clunk
{
    LayerStack::~LayerStack()
    {
        for(clLayer* layer : m_layers)
        {
            layer->OnDetach();
            delete layer;
        }
    }

    void LayerStack::Push(clLayer* Layer)
    {
        
    }

    void LayerStack::Pop(clLayer* clLayer)
    {
        
    }
}


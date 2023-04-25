#pragma once

#include <Defines/Defines.h>
#include <PCH/pch.h>

#include "ClLayer.h"

namespace Clunk
{
    class LayerStack
    {
    public:

        LayerStack() = default;
        ~LayerStack();

        void Push(clLayer* Layer);
        void Pop(clLayer* clLayer);

        std::vector<clLayer*>::iterator begin() { return m_layers.begin(); }
        std::vector<clLayer*>::iterator end() { return m_layers.end(); }
        std::vector<clLayer*>::reverse_iterator rbegin() { return m_layers.rbegin(); }
        std::vector<clLayer*>::reverse_iterator rend() { return m_layers.rend(); }

        std::vector<clLayer*>::const_iterator begin() const { return m_layers.begin(); }
        std::vector<clLayer*>::const_iterator end()	const { return m_layers.end(); }
        std::vector<clLayer*>::const_reverse_iterator rbegin() const { return m_layers.rbegin(); }
        std::vector<clLayer*>::const_reverse_iterator rend() const { return m_layers.rend(); }
        
    protected:

    private:

        std::vector<clLayer*> m_layers;

    };
}

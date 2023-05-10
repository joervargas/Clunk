#pragma once

#include <PCH/pch.h>
#include "ComponentRegistry.h"
#include "System.h"

namespace Clunk
{
    class Scene
    {
    public:

        CLUNK_INLINE Scene() {};

        CLUNK_INLINE ~Scene()
        {
            for(auto& sys : m_systems) { CLUNK_DELETE(sys); }
            m_systems.clear();
            m_compReg.Clear();
        }

        CLUNK_INLINE ECS::Entity AddEntity(const String& name)
        {
            ECS::Entity ent = ECS::Entity(&m_compReg);
            // TODO: Default components added here
            return ent;
        }

        /**
         * @brief Creates a system of type T. Pushes it to a vector of ISystem*.
         * @tparam T system type
         */
        template<typename T>
        CLUNK_INLINE void RegisterSystem()
        {
            FUSE_STATIC_ASSERT(std::is_base_of<ECS::ISystem, T>::value);
            auto new_system = new T();
            new_system->Prepare(&m_compReg);
            this->m_systems.push_back(new_system);
        }

        virtual void Start() {}
        virtual void Update(f32 DeltaTime) {}

    protected:

        std::vector<ECS::ISystem*> m_systems;
        ECS::ComponentRegistry m_compReg;

    };
}
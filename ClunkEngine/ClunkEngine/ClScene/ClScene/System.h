#pragma once

#include "Entity.h"
#include "ComponentRegistry.h"

namespace Clunk::ECS
{
    /**
     * @brief An interface struct for system objects. Derived structs/classes will 
     *   have to implement start() update(float deltatime), and destructor.
     */
    struct ISystem
    {

        CLUNK_INLINE virtual ~ISystem() = default;

        /**
         * @brief sets pointer handles to ComponentRegistry for internal use.
         * @param CReg ComponentRegistry*
         */
        CLUNK_INLINE void prepare(ComponentRegistry* CReg)
        {
            this->m_compReg = CReg;
        }

        /**
         * @brief Retrieves a std::vector<Entity> of all entities with component of type T.
         * @tparam T component type
         * @return std::vector<Entity>;
         */
        template<typename T>
        CLUNK_INLINE std::vector<Entity> View()
        {
            std::vector<Entity> entities;
            for(auto& e : m_compReg->View<T>())
            {
                entities.push_back(Entity(e, m_compReg));
            }
            return entities;
        }

        CLUNK_INLINE virtual void start() {};
        CLUNK_INLINE virtual void update(f32) {}


    protected:

        ComponentRegistry* m_compReg = nullptr;

    };
}
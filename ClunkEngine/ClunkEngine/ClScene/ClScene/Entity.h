#pragma once

#include "ComponentRegistry.h"

namespace Clunk::ECS
{
    /**
     * @brief A wrapper around entity_id for ease of use. Provides functions to handle entity specific operations
     */
    struct Entity
    {

        /**
         * @brief Entity constructor. Creates an entity from the registry
         * @param CR ComponentRegistry*
         */
        CLUNK_INLINE Entity(ComponentRegistry* CR) : m_compReg(CR)
            { m_entity = CR->AddEntity(); }

        /**
         * @brief Entity constructor. Constructs an entity object from an existing entity and regisry.
         * @param Id entity_id
         * @param CR ComponentRegistry*
         */
        CLUNK_INLINE Entity(entity_id Id, ComponentRegistry* CR) : 
            m_entity(Id), m_compReg(CR) {}

        /**
         * @brief Default constructor. Doesn't do anything.
         */
        CLUNK_INLINE Entity() = default;

        /**
         * @brief cast operator for retrieving the entity_id
         * @return entity_id 
         */
        CLUNK_INLINE operator entity_id() { return m_entity; }

       /**
         * @brief Adds a component<T> to the entity
         * @tparam T component type
         * @tparam Args component constructor arg types
         * @param args component constructor args
         * @return component<T>&
         */
        template<typename T, typename ... Args>
        CLUNK_INLINE T& AddComponent(Args&& ... args)
        {
            return m_compReg->AddComponent<T>(m_entity, std::forward<Args>(args) ... );
        }

        /**
         * @brief Get the component of type T
         * @tparam T component type
         * @return component<T>&
         */
        template<typename T>
        CLUNK_INLINE T& GetComponent()
        {
            return m_compReg->GetComponent<T>(m_entity);
        }

        /**
         * @brief Removes a component from entity
         * @tparam T component type
         */
        template<typename T>
        CLUNK_INLINE void RemoveComponent()
        {
            m_compReg->RemoveComponent<T>(m_entity);
        }

        /**
         * @brief returns true if entity has component<T>
         * @tparam T component type
         * @return bool
         */
        template<typename T>
        CLUNK_INLINE b8 HasComponent()
        {
            return m_compReg->HasComponent<T>(m_entity);
        }

        /**
         * @brief returns true is registry exists and entity contains component signatures.
         * @return bool
         */
        CLUNK_INLINE b8 IsAlive()
        {
            return m_compReg && m_compReg->IsAlive(m_entity);
        }

        /**
         * @brief Get entity_id
         * @return entity_id
         */
        CLUNK_INLINE entity_id Id() { return m_entity; }

        /**
         * @brief deletes components and components signatures of the entity
         */
        CLUNK_INLINE void destroy() { m_compReg->Destroy(m_entity); }
    
    private:
        entity_id m_entity = INVALID_ID;
        ComponentRegistry* m_compReg = nullptr;
    };
}
#pragma once

#include <PCH/pch.h>
#include "ECS.h"

namespace Clunk::ECS
{
    struct ComponentRegistry
    {
        /**
         * @brief Clear memory and reset entity counter.
         */
        CLUNK_INLINE void Clear()
        {
            for(auto& [_, a] : m_components) { CLUNK_DELETE(a); }
            m_components.clear();
            m_signatures.clear();
            m_nextId = INVALID_ID;
        }

        /**
         * @brief Adds a new entity to registry. Assigns a unique entity
         * @return entity_id of the new entity
         */
        CLUNK_INLINE entity_id AddEntity()
        {
            entity_id entity = m_nextId++;
            m_signatures[entity] = Signature();
            return entity;
        }

        /**
         * @brief Looks for components and signatures of the entity and deletes them.
         * @param Id entity_id to be destroyed
         */
        CLUNK_INLINE void Destroy(entity_id Id)
        {
            for(auto& [_, a] : m_components)
            {
                a->Erase(Id);
            }
            m_signatures.erase(Id);
        }

        /**
         * @brief returns false if the entity contains no component signatures (no components found).
         * @param Id entity_id
         * @return b8
         */
        CLUNK_INLINE b8 IsAlive(entity_id Id)
        {
            // returns zero if no components found.
            return m_signatures.count(Id);
        }

        /**
         * @brief Retrieves all entities with a component<T>
         * @tparam T component type
         * @return EntityList (std::set<entity_id>)
         */
        template<typename T>
        CLUNK_INLINE EntityList View()
        {
            EntityList list;
            for(auto& [entity, sig] : m_signatures)
            {
                if(sig.count(TypeID<T>()))
                {
                    list.insert(entity);
                    continue;
                }
            }
            return list;
        }

        /**
         * @brief Get the component from a given entity_id
         * @tparam T component type
         * @param Id entity_id
         * @return component<T>& 
         */
        template<typename T>
        CLUNK_INLINE T& GetComponent(entity_id Id)
        {
            CLUNK_ASSERT(m_signatures.count(Id) && "entity_id out of range ECS Signature!");
            return GetComponentArray<T>()->Get(Id);
        }

        /**
         * @brief Adds a component to an entity
         * @tparam T component type
         * @tparam Args component<T> constuctor arg types
         * @param Id entity_id
         * @param args component constructor args
         * @return component<T>& 
         */
        template<typename T, typename ... Args>
        CLUNK_INLINE T& AddComponent(entity_id Id, Args&& ... args)
        {
            CLUNK_ASSERT(m_signatures.count(Id) && "entity_id out of range ECS Signature!");
            T data = T(std::forward<Args>(args) ... );
            m_signatures[Id].insert(TypeID<T>());
            return GetComponentArray<T>()->Push(Id, data);
        }

        /**
         * @brief Removes a component from a given entity
         * @tparam T component type
         * @param Id entity_id
         */
        template<typename T>
        CLUNK_INLINE void RemoveComponent(entity_id Id)
        {
            m_signatures.at(Id).erase(TypeID<T>());
            GetComponentArray<T>()->Erase(Id);
        }

        /**
         * @brief Returns true if entity has a component<T>
         * @tparam T component type
         * @param e entityid
         * @return bool 
         */
        template<typename T>
        CLUNK_INLINE b8 HasComponent(entity_id Id)
        {
            return m_signatures.count(Id) && m_signatures.at(Id).count(TypeID<T>());
        }

        /**
         * @brief Get a specific ComponentArray of type T.
         * @tparam T component type
         * @return ComponentArray<T>* 
         */
        template<typename T>
        CLUNK_INLINE ComponentArray<T>* GetComponentArray()
        {
            component_id type = TypeID<T>();
            // if component type already exists
            if(m_components.count(type)) 
            {
                return static_cast<ComponentArray<T>*>(m_components.at(type));
            }
            // component type does not yet exist. Create it
            ComponentArray<T>* array = new ComponentArray<T>();
            // Set new ComponentArray<T> in m_components at index "type" (uint32_t)
            m_components[type] = array;
            return array;
        }

    private:

        /**
         * @brief A map of component arrays. Stores a specific component_array for each component_id.         * 
         */
        std::unordered_map<component_id, IArrayInstance*> m_components;

        /**
         * @brief A map that assigns to each existing entity a signature of components.
         *   If an entity has a component<T>, its component_id will be found here.
         */
        std::unordered_map<entity_id, Signature> m_signatures;

        /**
         * @brief Counter for entityids. Will increase every time a new entity is added.
         */
        entity_id m_nextId = INVALID_ID;

    };
}
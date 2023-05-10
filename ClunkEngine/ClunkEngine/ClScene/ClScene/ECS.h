#pragma once

#include <PCH/pch.h>


namespace Clunk::ECS
{
    using entity_id = size_t;
    using component_id = u32;

    /**
     * @brief A set of component_ids
     */
    using Signature = std::set<component_id>;

    /**
     * @brief A set of entity_ids
     */
    using EntityList = std::set<entity_id>;

    /**
     * @brief An interface type for ECS arrays
     * @fn virtual void Erase(entity_id) : must be implemented
     */
    struct IArrayInstance
    {
        CLUNK_INLINE virtual ~IArrayInstance() = default;
        virtual void Erase(entity_id) = 0;
    };

    /**
     * @brief ArrayEntry: Stores Component<T> and the entity_id it belongs to.
     * @tparam T  Component of type T
     */
    template<typename T>
    struct ArrayEntry
    {
        CLUNK_INLINE ArrayEntry(entity_id Id, const T& TypeData) :
            Entity(Id), Data(TypeData) {}

        // cast operator; used to retrieve entity_id
        CLUNK_INLINE operator entity_id() const { return Entity; }

        entity_id Entity = INVALID_ID;
        T Data;
    };

    template<typename T>
    struct ComponentArray : IArrayInstance
    {
        CLUNK_INLINE T& Push(entity_id Id, const T& TypeData)
        {
            if(Exists(Id)) { return Get(Id); }
            _data.push_back(ArrayEntry(Id, TypeData));
            return _data.back().Data;
        }

        CLUNK_INLINE void Erase(entity_id Id)
        {
            auto iter = std::find(_data.begin(), _data.end(), Id);
            if(iter != _data.end()) { _data.erase(iter); }
        }

        CLUNK_INLINE b8 Exists(entity_id Id)
        {
            return std::find(_data.begin(), _data.end(), Id) != _data.end();
        }

        CLUNK_INLINE T& Get(entity_id Id)
        {
            CLUNK_ASSERT(Exists(Id) && "entity_id out of range!");
            return(*std::find(_data.begin(), _data.end(), Id)).Data;
        }

    private:

        std::vector<ArrayEntry<T>> _data;

    };
}
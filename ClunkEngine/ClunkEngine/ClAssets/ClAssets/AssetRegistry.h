#pragma once

#include <PCH/pch.h>
#include "Asset.h"

namespace Clunk::Assets
{
    struct AssetRegistry
    {
        CLUNK_INLINE AssetRegistry() = default;

        /**
         * @brief Clears the registry
         */
        CLUNK_INLINE void Clear()
        {
            for(auto& [_, list] : m_data)
            {
                for(auto a : list) { CLUNK_DELETE(a); }
            }
            m_data.clear();
        }

        template<typename T>
        CLUNK_INLINE T* Get(asset_id Id)
        {
            CLUNK_STATIC_ASSERT(std::is_base_of<IAsset, T>::value);
            const auto type = TypeID<T>();
            if(!m_data.count(type)) { return nullptr; }

            for(IAsset& asset : m_data.at(type))
            {
                if(asset.Id) { return static_cast<T*>(asset); }
            }
            return nullptr;
        }

        template<typename T>
        CLUNK_INLINE T* Get(const String& Name)
        {
            CLUNK_STATIC_ASSERT(std::is_base_of<IAsset, T>::value);
            const auto type = TypeID<T>();
            if(!m_data.count(type)){ return nullptr; }

            for(auto& asset : m_data.at(type))
            {
                if(asset->Name == Name) { return static_cast<T*>(asset); }
            }
            return nullptr;
        }

        template<typename T>
        CLUNK_INLINE asset_id GetId(const String& Name)
        {
            CLUNK_STATIC_ASSERT(std::is_base_of<IAsset, T>::value);
            const auto type = TypeID<T>();
            if(!m_data.count(type)){ return INVALID_ID; }

            for(auto& asset : m_data.at(type))
            {
                if(asset->Name == Name) { return asset->Id; }
            }
            return INVALID_ID;
        }

        template<typename T>
        CLUNK_INLINE T* Add(const String& Name)
        {
            auto asset = new T();
            asset->Name = Name;
            m_data[TypeID<T>()].push_back(asset);
            return asset;
        }

    private:

        std::unordered_map<asset_type_id, std::vector<IAsset*>> m_data;
    };
}
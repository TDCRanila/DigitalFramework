#pragma once

#include <AnyStorage/AnyStorage.h>

#include <unordered_map>
#include <functional>
#include <any>

namespace Any 
{
    class AnyStorageManager 
	{
    public:
        AnyStorageManager() = default;
        ~AnyStorageManager() = default;

        template <typename T>
        void Subscribe(T* a_item);

        template <typename T>
		AnyStorage<T>* GetItemVector() const;

    private:
        template <typename T>
        AnyStorage<T>* AddItemVector(size_t a_hash);
 
		std::unordered_map<size_t, std::any> _storage_map;

    };

#pragma region Template Function Implementation

    template <typename T>
    void AnyStorageManager::Subscribe(T* a_item) 
	{
        // Search for item vector in storage.
		AnyStorage<T>* storage_ptr = GetItemVector<T>();
        if (!storage_ptr)
		{
            size_t current_hash = typeid(T).hash_code();
            storage_ptr         = AddItemVector<T>(current_hash);
        }

        // Add element
        storage_ptr.emplace_back(a_item);
    }

    template <typename T>
	AnyStorage<T>* AnyStorageManager::AddItemVector(size_t a_hash) 
	{
		AnyStorage<T>* temp = new AnyStorage<T>();
        _storage_map.insert(std::make_pair(a_hash, temp));
        return temp;
    }

    template <typename T>
	AnyStorage<T>* AnyStorageManager::GetItemVector() const
	{
        // Search for item vector in storage.
        size_t current_hash         = typeid(T).hash_code();
		AnyStorage<T>* storage_ptr  = nullptr;
        if (auto [hash, any_object] = _storage_map.find(current_hash); it != _storage_map.end())
		{
            storage_ptr = std::any_cast<AnyStorage<T>*>(any_object);
        }
        else
        {
            return storage_ptr;
        }
    }

#pragma endregion

} // End of namespace ~ Any

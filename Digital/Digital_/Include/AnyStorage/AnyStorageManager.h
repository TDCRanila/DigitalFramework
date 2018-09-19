#pragma once

#include <AnyStorage/AnyStorage.h>

#include <map>
#include <functional>
#include <any>

namespace DAnyStorage {

    class AnyStorageManager {
    public:
		AnyStorageManager()		{ /*Empty*/ }
        ~AnyStorageManager()	{ /*Empty*/ }

        template <class T>
        void Subscribe(T* a_item);


        template <class T>
		AnyStorage<T>* GetItemVector();

    private:
        template <class T>
        AnyStorage<T>* AddItemVector(size_t a_hash);
 
		std::unordered_map<size_t, std::any> storage;

    };

#pragma region Template Function Implementation

    template <class T>
    void AnyStorageManager::Subscribe(T* a_item) {
        // Search for item vector in storage.
		AnyStorage<T>* i = nullptr;
        i = GetItemVector<T>();

        // If Not ~
        if (i == nullptr) {
            size_t current_hash = typeid(T).hash_code();
            i = AddItemVector<T>(current_hash);
        }

        // Add element
        i->item_vector.emplace_back(a_item);
    }

    template <class T>
	AnyStorage<T>* AnyStorageManager::AddItemVector(size_t a_hash) {
		AnyStorage<T>* temp = new AnyStorage<T>();
        this->storage.insert(std::make_pair(a_hash, temp));
        return temp;
    }

    template <class T>
	AnyStorage<T>* AnyStorageManager::GetItemVector() {
        // Check storage is empty. Is faster? How many instructions.

        // Hash
        size_t current_hash = typeid(T).hash_code();

        // Search for item vector in storage.
		AnyStorage<T>* storage_ptr = nullptr;
        auto it = this->storage.find(current_hash);
        if (it != this->storage.end()) {
            storage_ptr = std::any_cast<AnyStorage<T>*>(it->second);
        }

        return storage_ptr;
    }

#pragma endregion

} // End of namespace ~ DAnyStorage

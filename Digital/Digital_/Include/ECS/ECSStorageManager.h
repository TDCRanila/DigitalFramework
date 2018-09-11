#pragma once

#include <ECS/ECSStorage.h>

#include <map>
#include <functional>
#include <any>

namespace Digital {

    class ECSStorageManager {
    public:
        ECSStorageManager();
        ~ECSStorageManager();

        template <class T>
        void Subscribe(T* a_component);

        template <class T>
        ECSStorage<T>* AddComponentVector(size_t a_hash);

        template <class T>
        ECSStorage<T>* GetComponentVector();

    private:
        std::unordered_map<size_t, std::any> storage;

    };

#pragma region Template Function Implementation

    template <class T>
    void ECSStorageManager::Subscribe(T* a_component) {
        // Search for component vector in storage.
        ECSStorage<T>* c = nullptr;
        c = GetComponentVector<T>();

        // If Not ~
        if (c == nullptr) {
            size_t current_hash = typeid(T).hash_code();
            c = AddComponentVector<T>(current_hash);
        }

        // Add element
        c->component_vector.emplace_back(a_component);
    }

    template <class T>
    ECSStorage<T>* ECSStorageManager::AddComponentVector(size_t a_hash) {
        ECSStorage<T>* temp = new ECSStorage<T>();
        this->storage.insert(std::make_pair(a_hash, temp));
        return temp;
    }

    template <class T>
    ECSStorage<T>* ECSStorageManager::GetComponentVector() {
        // Check storage is empty. Is faster? How many instructions.

        // Hash
        size_t current_hash = typeid(T).hash_code();

        // Search for component vector in storage.
        ECSStorage<T>* storage_ptr = nullptr;
        auto it = this->storage.find(current_hash);
        if (it != this->storage.end()) {
            storage_ptr = std::any_cast<ECSStorage<T>*>(it->second);
        }

        return storage_ptr;
    }

#pragma endregion

} // End of namespace ~ Digital

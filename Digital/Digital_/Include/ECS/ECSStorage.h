#pragma once

#include <vector>

namespace Digital {

    template <class T>
    class ECSStorage {
    public:
        ECSStorage() {

        }

        ~ECSStorage() {

        }

        std::vector<T*> component_vector;

    };

} // End of namespace ~ Digital
#pragma once

#include <vector>

namespace Digital {

    template <class T>
    class AnyStorage {
    public:
		AnyStorage() {

        }

        ~AnyStorage() {

        }

        std::vector<T*> item_vector;

    };

} // End of namespace ~ Digital
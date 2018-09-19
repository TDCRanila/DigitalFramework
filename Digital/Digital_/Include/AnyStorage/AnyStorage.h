#pragma once

#include <vector>

namespace DAnyStorage {

    template <class T>
    class AnyStorage {
    public:
		AnyStorage() { /*Empty*/ }

        ~AnyStorage() { /*Empty*/ }

        std::vector<T*> item_vector;

    };

} // End of namespace ~ DAnyStorage

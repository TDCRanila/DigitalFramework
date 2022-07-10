#pragma once

#include <Utility/TemplateUtility.h>

namespace DFW
{
    namespace DUtility
    {
        using FamilyTypeID = int32;
        constexpr FamilyTypeID DFW_INVALID_FAMILY_TYPE_ID = -1;

        template <typename BaseFamilyType>
        class FamilyType
        {
        public:
            FamilyType() = default;
            ~FamilyType() = default;

            template <typename ClassType>
            static FamilyTypeID GetTypeID();
            static size_t GetTypeIDCount();
        
        private:
            static FamilyTypeID _type_id_count;

        };

        template <typename BaseFamilyType>
        template <typename DerivedType>
        FamilyTypeID FamilyType<BaseFamilyType>::GetTypeID()
        {
            if constexpr (AreSameTypes<BaseFamilyType, DerivedType>)
            {
                return FamilyTypeID(0);
            }
            else if constexpr (IsDerivedFrom<DerivedType, BaseFamilyType>)
            {
                static const FamilyTypeID type_id(++_type_id_count);
                return type_id;
            }
            else
            {
                static_assert(IsAlwaysFalse<DerivedType>, __FUNCTION__ " - Trying to get FamilyTypeID of Type, but it isn't derived from the BaseFamilyType.");
                return DFW_INVALID_FAMILY_TYPE_ID;
            }
        }

        template <typename BaseFamilyType>
        size_t FamilyType<BaseFamilyType>::GetTypeIDCount()
        {
            // BaseType + All DerivedTypes.
            return 1 + _type_id_count;
        }

        template <typename BaseFamilyType>
        FamilyTypeID FamilyType<BaseFamilyType>::_type_id_count = 0;

    } // End of namespace DUtility.

} // End of namespace ~ DFW.

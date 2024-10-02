#pragma once

#include <DFW/Utility/TemplateUtility.h>

namespace DFW
{
    namespace DUtility
    {
        using FamilyTypeID = int32;
        constexpr FamilyTypeID DFW_INVALID_FAMILY_TYPE_ID = -1;

        /// <summary>
        /// This class provides an easy way to create TypeIDs for a given family name. 
        /// The base family name is the container for its derived family names. Each family name
        /// has a TypeID assigned to it.
        /// </summary>
        /// <typeparam name="base_family_name"></typeparam>
        template <StringLiteral base_family_name>
        class FamilyNameType
        {
        public:
            FamilyNameType() = default;
            ~FamilyNameType() = default;

            template <StringLiteral derived_family_name>
            static FamilyTypeID GetTypeID();
            static size_t GetTypeIDCount();

        private:
            static FamilyTypeID _type_id_count;

        };

        template <StringLiteral base_family_name>
        template <StringLiteral derived_family_name>
        FamilyTypeID FamilyNameType<base_family_name>::GetTypeID()
        {
            if constexpr (base_family_name.value == derived_family_name.value)
            {
                return FamilyTypeID(0);
            }
            else
            {
                static const FamilyTypeID type_id(++_type_id_count);
                return type_id;
            }
        }

        template <StringLiteral base_family_name>
        size_t FamilyNameType<base_family_name>::GetTypeIDCount()
        {
            // BaseType + All DerivedTypes.
            return 1 + _type_id_count;
        }

        template <StringLiteral base_family_name>
        FamilyTypeID FamilyNameType<base_family_name>::_type_id_count = 0;

        /// <summary>
        /// This class provides an easy way to create TypeIDs for each derived class types which 
        /// are derived from the base class. 
        /// The base family class type is the container for its derived classes. Each family class
        /// has a TypeID assigned to it.
        /// </summary>
        /// <typeparam name="BaseFamilyType"></typeparam>
        template <typename BaseFamilyType>
        class FamilyClassType
        {
        public:
            FamilyClassType() = default;
            ~FamilyClassType() = default;

            template <typename ClassType>
            static FamilyTypeID GetTypeID();
            static size_t GetTypeIDCount();
        
        private:
            static FamilyTypeID _type_id_count;

        };

        template <typename BaseFamilyType>
        template <typename DerivedType>
        FamilyTypeID FamilyClassType<BaseFamilyType>::GetTypeID()
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
        size_t FamilyClassType<BaseFamilyType>::GetTypeIDCount()
        {
            // BaseType + All DerivedTypes.
            return 1 + _type_id_count;
        }

        template <typename BaseFamilyType>
        FamilyTypeID FamilyClassType<BaseFamilyType>::_type_id_count = 0;

    } // End of namespace DUtility.

} // End of namespace ~ DFW.

#pragma once

#include <AutoFactory/AutoFactory.h>

#include <Utility/TemplateUtility.h>

namespace DCore
{
    // FW Declare
    class ReflectedEvent;

    template <typename FactoryType>
    concept IsFactoryType = requires(FactoryType f) { FactoryType::GetFactories(); };

    class EventLibrary
    {
    public:
        ~EventLibrary() = default;

        template <typename FactoryType>
        requires (IsDerivedFrom<FactoryType, ReflectedEvent>)
        static void ProcessEventCollection();

    private:
        EventLibrary() = default;

        inline static int32 _category_counter = 0;
    };

} // End of namespace ~ DCore.

#pragma region Template Implementation

namespace DCore
{
    template <typename FactoryType>
    requires (IsDerivedFrom<FactoryType, ReflectedEvent>)
    void EventLibrary::ProcessEventCollection()
    {
        static_assert(IsFactoryType<FactoryType>, "FactoryType is not derived from AutoFactory and does not contain GetFactories();");

        int32 _type_counter = 0;
        for (auto const& [type_pair, fac] : FactoryType::GetFactories())
        {
            // Set Static Event Type ID.
            fac()->_event_type_id = (_category_counter << 8) + _type_counter;
            ++_type_counter;
        }
        ++_category_counter;
    }

} // End of namespace ~ DCore.

#pragma endregion
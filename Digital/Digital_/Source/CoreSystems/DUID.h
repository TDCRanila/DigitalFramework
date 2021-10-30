#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/nil_generator.hpp>
#include <boost/container_hash/hash.hpp>

namespace DCore
{
	auto const DFW_INVALID_DUID = boost::uuids::nil_generator()();

	class DUID final : public boost::uuids::uuid
	{
	public:
		DUID()
			: boost::uuids::uuid(boost::uuids::nil_generator()())
		{
		}

		explicit DUID(boost::uuids::uuid const& a_uuid)
			: boost::uuids::uuid(a_uuid)
		{
		}

		operator boost::uuids::uuid()
		{
			return static_cast<boost::uuids::uuid&>(*this);
		}

		operator boost::uuids::uuid() const
		{
			return static_cast<boost::uuids::uuid const&>(*this);
		}

		operator std::string() const
		{
			return boost::uuids::to_string(*this);
		}

		std::strong_ordering operator<=>(DUID const& a_other) const = default;

	};

	inline DUID GenerateDUID()
	{
		return DUID { boost::uuids::random_generator()() };
	}

} // End of Namespace ~ Digital.

namespace std
{
	// Custom std::hash functionality for DUID.
	template<>
	struct hash<DCore::DUID>
	{
		std::size_t operator()(DCore::DUID const& a_duid) const
		{
			std::size_t hash_val = boost::hash<DCore::DUID>()(a_duid);
			return hash_val;
		}
	};
}

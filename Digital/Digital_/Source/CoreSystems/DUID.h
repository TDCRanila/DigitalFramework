#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/nil_generator.hpp>
#include <boost/container_hash/hash.hpp>

namespace DCore
{
	class InvalidDUID : public boost::uuids::uuid
	{
	public:
		InvalidDUID()
			: boost::uuids::uuid(boost::uuids::nil_generator()())
		{
		}
	};

	class DUID : public boost::uuids::uuid
	{
	public:
		DUID()
			: boost::uuids::uuid(boost::uuids::nil_generator()())
		{
		}

		explicit DUID(boost::uuids::uuid const& u)
			: boost::uuids::uuid(u)
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
	};

	class DUIDGenerator
	{
	public:
		static DUID GenerateID()
		{
			DUID id;
			boost::uuids::uuid random_id = boost::uuids::random_generator()();
			id.swap(random_id);
			return id;
		}
	};

} // End of Namespace ~ Digital.

namespace std
{
	// Custom std::hash functionality for DUID.
	template<>
	struct hash<DCore::DUID>
	{
		std::size_t operator()(const DCore::DUID& a_duid) const
		{
			boost::hash<DCore::DUID> duid_hasher;
			std::size_t hash_val = duid_hasher(a_duid);
			return hash_val;
		}
	};
}

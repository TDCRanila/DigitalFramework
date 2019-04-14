#include <Utility/StringUtility.h>

namespace DUtility
{

	bool SubtractString(std::string& a_original_string, std::string a_string_to_remove)
	{
		std::size_t pos = a_original_string.find(a_string_to_remove);
		if (pos == std::string::npos)
		{
			return false;
		}
		else
		{
			a_original_string.erase(pos, a_string_to_remove.length());
			return true;
		}
	}

} // End of namespace ~ DUtility

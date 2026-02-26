#include <DFW/Utility/StringUtility.h>

namespace DFW
{
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

		void FindAndRemoveChar(std::string& a_original_string, char a_char_to_remove)
		{
			std::string& s = a_original_string;
			s.erase(std::remove_if(s.begin(), s.end(), [&a_char_to_remove](char& c) { return c == a_char_to_remove; }), s.end());
		}

		bool FindAndReplaceChar(std::string& a_original_string, char a_char_to_remove, char a_char_to_replace)
		{
			bool has_replaced = false;
			for (char& c : a_original_string)
			{
				if (c == a_char_to_remove)
				{
					has_replaced = true;
					c = a_char_to_replace;
				}
			}
			return has_replaced;
		}

		bool ContainsChar(const std::string a_string, char a_chararcter)
		{
			return a_string.find(a_chararcter) != std::string::npos;
		}

		bool StringToBool(std::string a_string)
		{
			if (a_string == "0" || a_string == "FALSE") { return false; }
			else if (a_string == "1" || a_string == "TRUE") { return true; }
			else { return false; }
		}

	} // End of namespace ~ DUtility

} // End of namespace ~ DFW.
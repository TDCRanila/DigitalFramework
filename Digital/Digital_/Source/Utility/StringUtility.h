#pragma once

#include <string>

namespace DUtility 
{
    /**
    *   Subtract a string from the original string.
    *   @param std::string a_string_to_remove The string that you want to remove from the origonal string.
    *   @param std::string& a_original_string The original string, which be will returned.
    *   @return bool True ~ if there where no errors - False ~ if there errors.
    */
	bool SubtractString(std::string& a_original_string, std::string a_string_to_remove);

    void FindAndRemoveChar(std::string& a_original_string, char a_char_to_remove);

    bool FindAndReplaceChar(std::string& a_original_string, char a_char_to_remove, char a_char_to_replace);

    /*
    *   Check the given string, if it contains a certain chararacter.
    *	@param std::string String that you want to check.
    *	@param	char a_character The char that you want to check.
    *	@return bool True ~ if it could find the specified char - False ~ if it couldn't find the specifed char.
    */
    bool ContainsChar(const std::string a_string, char a_chararcter);

    /*
    *   Converts a string to a bool. Defaults to false if the string was incorrect.
    *	@detail (0 = false) & (1 = true)
    *	@param std::string a_string The string that you want to check
    *	@return bool True ~ if the string was true - False ~ if the string was false.
    */
    bool StringToBool(std::string a_string);

} // End of namespace ~ DUtility

#pragma once

#include <string>
#include <vector>

namespace DFW
{
    namespace DUtility
    {
        /*
        *   Loads and returns vector of File Paths in a specified directory relative to the working directory.
        *	@param	std::string a_directory_path The filepath of the directory that you want to check.
        *	@return std::vector<std::string> Vector of directory names.
        */
        std::vector<std::string> LoadFilesInDirectory(std::string a_directory_path);

        /*
        *   Checks whether a file exist in a directory relative to the working directory.
        *	@param std::string a_directory_path The filepath of the directory that you want to check.
        *   @param std::string a_file The name of the file path that you want to search for.
        *	@return bool True ~ if the file has been found - False ~ if the file cannot be found.
        */
        bool DoesFileExistInDir(std::string a_directory_path, std::string a_file);

        /*
        *   Creates a directory relative to the working directory. Also works if the directory already exists.
        *	@param std::string a_directory_path The path of the new directory.
        *   @return bool True ~ if the file got created - False ~ if the file it did't get created.
        */
        bool CreateNewDirectory(std::string a_directory_path);

        /*
        *   Checks if a directory, relative to the working directory, exists.
        *	@param std::string a_directory_path The path of the directory.
        *   @return bool True ~ if the directory exists - False ~ if the directory doesn't exist.
        */
        bool DoesDirectoryExist(std::string a_directory_path);

        /**
        *   Gets the path of the working directory.
        *   @param std::string& a_returned_path The returned path.
        *   @return bool True ~ if there where no errors - False ~ if there errors.
        */
        bool GetWorkDirectory(std::string& a_directory_path);

        /**
        *   Gets the parent path of a file
        *   @param std::string& a_returned_path The returned path.
        *   @return bool True ~ if there where no errors - False ~ if there errors.
        */
        std::string GetParentPath(std::string a_file_path);

        /*
        *   Removes all of the line ending of the string that is passed through.
        *	@param std::string& a_string The string that you want to modify.
        */
        void RemoveLineEndings(std::string& a_string);

        /*
        *   Check if the files exists and removes the file.
        *	@param std::string a_file_path The path of the file.
        *   @return bool True ~  if the file got deleted - False ~ if it the file did't get deteled.
        */
        bool FSDeleteFile(std::string a_file_path);

        /*
        *   Parses the given file path to get the name and extension of the file.
        *	@example Resources\\Asset\\file.abc -> file.abc)
        *	@param std::string a_file_path String of the filepath.
        *	@return std::string The parsed string.
        */
        std::string GetFileName(std::string a_file_path);

        /*
        *   Parses the given filePath to get the name and extension.
        *	@example file.abc -> file)
        *	@param std::string a_file_path String of the filepath.
        *	@return The parsed string.
        */
        std::string GetFileStem(std::string a_file_path);

        /*
        *   Returns the file extension of a given file.
        *	@param std::string a_string File path of the file.
        *	@return std::string Returns a string of the file extension.
        */
        std::string GetFileExtension(std::string a_string);

        /*
        *   Loads all content from a file into a std::istringstream.
        *   std::istringstream is created on the heap, so make sure to delete this pointer when you're done with it.
        *	@param a_path The path (with filename) to the file
        *	@return std::istringstream pointer. Use 'delete' to delete this pointer after using it.
        */
        std::istringstream* GetFileData(const std::string& a_path);

    } // End of namespace ~ DUtility

} // End of namespace ~ DFW.

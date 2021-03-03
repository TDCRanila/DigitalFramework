#include <Utility/FileSystemUtility.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

namespace DUtility {

#if defined (DFW_PLATFORM_WINDOWS)

    std::vector<std::string> LoadFilesInDirectory(std::string a_directory_path) 
    {
        std::vector<std::string> file_path;
        std::stringstream buffer;
        std::string temp;

        fs::directory_iterator dir_it(a_directory_path);
        for (auto& directory_entry : dir_it) 
        {
            buffer << directory_entry << std::endl;
            temp = buffer.str();

            DUtility::RemoveLineEndings(temp);
            file_path.emplace_back(temp);

            buffer.str(std::string()); // Clearing sstream
        }

        return file_path;
    }

    bool GetWorkDirectory(std::string& a_directory_path) 
    {
        std::error_code error;
        fs::path path(fs::current_path(error));
        if (error.value()) 
        {
            std::cerr << error.message() << std::endl;
            return false;
        } 
        else 
        {
            a_directory_path = path.string();
            return true;
        }
    }

    std::string GetParentPath(std::string a_file_path) 
    {
        std::error_code error;
        fs::path path(a_file_path);
        if (error.value()) 
        {
            std::cerr << error.message() << std::endl;
            return std::string("");
        }
        return path.parent_path().string();
    }

    bool DoesFileExistInDir(std::string a_directory_path, std::string a_file) 
    {
        std::stringstream buffer;
        std::string compare_path;
        std::string full_file_path_string = std::string(a_directory_path + DIR_SLASH + a_file);

        fs::directory_iterator dir_it(a_directory_path);
        for (auto& directory_entry : dir_it)
        {
            buffer << directory_entry << std::endl;
            compare_path = buffer.str();
            RemoveLineEndings(compare_path);

            if (compare_path == full_file_path_string)
                return true;

            buffer.str(std::string()); // Clearing sstream
        }

        return false;
    }

    bool CreateNewDirectory(std::string a_directory_path) 
    {
        std::error_code error;
        fs::create_directory(a_directory_path, error);
        if (error.value()) 
        {
            std::cerr << error.message() << std::endl;
            return false;
        } 
        else 
        {
            return true;
        }
    }

    bool DoesDirectoryExist(std::string a_directory_path) 
    {
        return fs::exists(a_directory_path);
    }

    void RemoveLineEndings(std::string& a_string) 
    {
        a_string.erase(std::remove(a_string.begin(), a_string.end(), '\n'), a_string.end());
        a_string.erase(std::remove(a_string.begin(), a_string.end(), '\r'), a_string.end());
    }

    bool FSDeleteFile(std::string a_file_path) 
    {
        RemoveLineEndings(a_file_path);
        std::error_code error;
        if (fs::exists(a_file_path, error)) 
        {
            fs::remove(a_file_path, error);
        }

        if (error.value()) 
        {
            std::cerr << error.message() << std::endl;
            return false;
        } 
        else 
        {
            return true;
        }
    }

    std::string GetFileName(std::string a_file_path) 
    {
        std::stringstream buffer;

        buffer << fs::path(a_file_path).filename();
        return (buffer.str());
    }

    std::string GetFileStem(std::string a_file_path) 
    {
        std::stringstream buffer;

        buffer << fs::path(a_file_path).stem();
        return (buffer.str());
    }

    std::string GetFileExtension(std::string a_string) 
    {
        return fs::path(a_string).extension().string();
    }

    std::istringstream* GetFileData(const std::string &a_path) 
    {
        std::ifstream ifs(a_path.c_str());
        std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

        return new std::istringstream(str);
    }

#else // OTHER SYSTEMS

    std::vector<std::string> LoadFilesInDirectory(std::string a_directory_path) 
    {
        std::vector<std::string> file_path;

        return file_path;
    }

    bool DoesFileExistInDir(std::string a_directory_path, std::string a_file) 
    {
        return false;
    }

    bool CreateNewDirectory(std::string a_directory_path) 
    {
        return true;
    }

    bool DoesDirectoryExist(std::string a_directory_path) 
    {
        return false;
    }

    void RemoveLineEndings(std::string& a_string) 
    {
        a_string.erase(std::remove(a_string.begin(), a_string.end(), '\n'), a_string.end());
        a_string.erase(std::remove(a_string.begin(), a_string.end(), '\r'), a_string.end());
    }

    bool DeleteFile(std::string a_file_path) 
    {
        RemoveLineEndings(a_file_path);
        return false;
    }


    std::string GetFileName(std::string a_file_path) 
    {
        return ("");
    }

    std::string GetFileStem(std::string a_file_path) 
    {
        return ("");
    }

    std::string GetFileExtension(std::string a_string) 
    {
        return ("");
    }

    bool ContainsChar(const std::string a_string, char a_chararcter) 
    {
        return a_string.find(a_chararcter) != std::string::npos;
    }

    bool StringToBool(std::string a_string) 
    {
        if      (a_string == "0") { return false; }
        else if (a_string == "1") { return true; }
        else                      { return false; }
    }

    std::istringstream* GetFileData(const std::string& a_path) 
    {
        return new std::istringstream("");
    }

#endif // Other Systems

} // End of namspace ~ DUtility.

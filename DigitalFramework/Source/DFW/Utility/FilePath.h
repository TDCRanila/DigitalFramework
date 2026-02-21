#pragma once

#include <filesystem>

namespace DFW
{
    class FilePath final : public std::filesystem::path
    {
    public:
        FilePath() = default;
        FilePath(std::filesystem::path const& a_path) : std::filesystem::path(a_path) {}
        ~FilePath() = default;
                
        operator std::string() const { return string(); }
        std::string GetFileName() const { return filename().string(); }
        std::string GetFileExtension() const { return extension().string(); }
        std::string GetFileNameWithoutExtension() const { return stem().string(); }

        bool IsValidPath() const { return std::filesystem::exists(*this); }
        bool IsDirectory() const { return std::filesystem::is_directory(*this); }
        bool IsFile() const { return !IsDirectory(); }

    };

} // End of namespace ~ DFW.

#pragma once

#include <array>
#include <functional>
#include <utility>

namespace DFW
{
    namespace DUtility
    {
        template <typename DataType, size_t Width, size_t Height>
        struct StaticGrid2D
        {
        public:
            StaticGrid2D() : width(Width), height(Height) { }

            DataType const& Get(size_t const a_x, size_t const a_y) const;
            DataType& Get(size_t const a_x, size_t const a_y);

            std::array<DataType, Height> GetColumn(size_t const a_x) const;
            std::array<DataType, Width> GetRow(size_t const a_y) const;

            std::array<DataType*, Height> GetColumnRef(size_t const a_x);
            std::array<DataType*, Width> GetRowRef(size_t const a_y);

        public:
            size_t GetIndex(size_t const a_x, size_t const a_y) const { return a_x + a_y * width; }
            size_t GetXCoordinate(size_t const a_index) const { return a_index % width; }
            size_t GetYCoordinate(size_t const a_index) const { return a_index / width; }
            bool IsValidCoordinate(size_t const a_x, size_t const a_y) const;
                
            std::array<DataType, Width * Height> data{ DataType() };
            size_t const width;
            size_t const height;

        };

#pragma region Template Function Implementation

        template <typename DataType, size_t Width, size_t Height>
        std::array<DataType, Height> StaticGrid2D<DataType, Width, Height>::GetColumn(size_t const a_x) const
        {
            DFW_ASSERT(a_x >= 0 && a_x < Width && "Attempting to access out-of-range elements with this coordinate.");

            std::array<DataType, Height> copy_array{ DataType() };
            for (size_t index(0); index < Height; index++)
                copy_array[index] = data[GetIndex(a_x, index)];

            return std::move(copy_array);
        }

        template <typename DataType, size_t Width, size_t Height>
        std::array<DataType, Width> StaticGrid2D<DataType, Width, Height>::GetRow(size_t const a_y) const
        {
            DFW_ASSERT(a_y >= 0 && a_y < Height && "Attempting to access out-of-range elements with this coordinate.");

            std::array<DataType, Width> copy_array{ DataType() };
            for (size_t index(0); index < Width; index++)
                copy_array[index] = data[GetIndex(index, a_y)];

            return std::move(copy_array);
        }

        template <typename DataType, size_t Width, size_t Height>
        std::array<DataType*, Height> StaticGrid2D<DataType, Width, Height>::GetColumnRef(size_t const a_x)
        {
            DFW_ASSERT(a_x >= 0 && a_x < Width && "Attempting to access out-of-range elements with this coordinate.");

            std::array<DataType*, Height> copy_array{ nullptr };
            for (size_t index(0); index < Height; index++)
                copy_array[index] = &data[GetIndex(a_x, index)];

            return std::move(copy_array);
        }

        template <typename DataType, size_t Width, size_t Height>
        std::array<DataType*, Width> StaticGrid2D<DataType, Width, Height>::GetRowRef(size_t const a_y)
        {
            DFW_ASSERT(a_y >= 0 && a_y < Height && "Attempting to access out-of-range elements with this coordinate.");

            std::array<DataType*, Width> copy_array{ nullptr };
            for (size_t index(0); index < Width; index++)
                copy_array[index] = &data[GetIndex(index, a_y)];

            return std::move(copy_array);
        }

        template <typename DataType, size_t Width, size_t Height>
        DataType const& StaticGrid2D<DataType, Width, Height>::Get(size_t const a_x, size_t const a_y) const 
        { 
            DFW_ASSERT(a_x >= 0 && a_x < Width && "Attempting to access out-of-range element with this coordinate."); 
            DFW_ASSERT(a_y >= 0 && a_y < Height && "Attempting to access out-of-range element with this coordinate.");
            return data[GetIndex(a_x, a_y)]; 
        }

        template <typename DataType, size_t Width, size_t Height>
        DataType& StaticGrid2D<DataType, Width, Height>::Get(size_t const a_x, size_t const a_y) 
        { 
            return const_cast<DataType&>(std::as_const(*this).Get(a_x, a_y)); 
        }

        template <typename DataType, size_t Width, size_t Height>
        bool StaticGrid2D<DataType, Width, Height>::IsValidCoordinate(size_t const a_x, size_t const a_y) const
        {
            return ((a_x >= 0) && (a_x < Width) && (a_y >= 0) && (a_y < Height));
        }

#pragma endregion

    } // End of namespace ~ DUtility.

} // End of namespace ~ DFW.
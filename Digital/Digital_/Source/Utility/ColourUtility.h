#pragma once

#include <glm/fwd.hpp>
#include <glm/detail/type_vec4.hpp>
#include <glm/detail/type_vec3.hpp>

namespace DFW
{
    struct ColourRGBA final : public glm::u8vec4
    {
	public:
        ColourRGBA() : glm::u8vec4(0, 0, 0, 0) {}
		ColourRGBA(uint8 a_red, uint8 a_green, uint8 a_blue, uint8 a_alpha) : glm::u8vec4(a_red, a_green, a_blue, a_alpha) {}
		ColourRGBA(uint8 a_red, uint8 a_green, uint8 a_blue) : glm::u8vec4(a_red, a_green, a_blue, 255) {}
		ColourRGBA(glm::u8vec3 a_rgb) : glm::u8vec4(a_rgb.r, a_rgb.g, a_rgb.b, 255.f) {}
        ~ColourRGBA() = default;

        float32 GetRComponentFloat() const { return r / 255.f; }
        float32 GetGComponentFloat() const { return g / 255.f; }
        float32 GetBComponentFloat() const { return b / 255.f; }
        float32 GetAComponentFloat() const { return a / 255.f; }
        glm::vec4 GetRGBAFloat() const { return glm::vec4(r, g, b, a) / 255.f; }
		uint32 GetABGRHex() const { return ((static_cast<uint32>(a) & 0xff) << 24) + ((static_cast<uint32>(b) & 0xff) << 16) + ((static_cast<uint32>(g) & 0xff) << 8) + ((static_cast<uint32>(r) & 0xff)); }
		uint32 GetRGBAHex() const { return ((static_cast<uint32>(r) & 0xff) << 24) + ((static_cast<uint32>(g) & 0xff) << 16) + ((static_cast<uint32>(b) & 0xff) << 8) + ((static_cast<uint32>(a) & 0xff)); }

		void RandomizeRGB();
		void RandomizeRGBA();

	public:
		static const ColourRGBA Black;
		static const ColourRGBA DarkRed;
		static const ColourRGBA Red;
		static const ColourRGBA DarkGreen;
		static const ColourRGBA Green;
		static const ColourRGBA DarkBlue;
		static const ColourRGBA Blue;
		static const ColourRGBA Yellow;
		static const ColourRGBA Purple;
		static const ColourRGBA Cyan;
		static const ColourRGBA Orange;
		static const ColourRGBA DarkOrange;
		static const ColourRGBA Grey;
		static const ColourRGBA DarkGrey;
		static const ColourRGBA LightGrey;
		static const ColourRGBA White;

    };

	ColourRGBA RandomColour();
	ColourRGBA RandomColourRGBA();

} // End of namespace ~ DFW.

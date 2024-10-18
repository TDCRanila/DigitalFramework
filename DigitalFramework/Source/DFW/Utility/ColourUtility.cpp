#include <DFW/Utility/ColourUtility.h>

#include <DFW/Utility/RandomUtility.h>

#include <DFW/Defines/MathDefines.h>

#include <cmath>

namespace DFW
{
    void ColourRGBA::RandomizeRGB()
    {
        this->r = DUtility::RandomIntRange<uint8>(0, 255);
        this->g = DUtility::RandomIntRange<uint8>(0, 255);
        this->b = DUtility::RandomIntRange<uint8>(0, 255);
    }

    void ColourRGBA::RandomizeRGBA()
    {
        RandomizeRGB();
        this->a = DUtility::RandomIntRange<uint8>(0, 255);
    }

    ColourRGBA ColourRGBA::Mix(ColourRGBA const& a_other_colour) const
    {
        return ColourRGBA(
            static_cast<uint8>(std::floor((r + a_other_colour.r) * 0.5f)),
            static_cast<uint8>(std::floor((g + a_other_colour.g) * 0.5f)),
            static_cast<uint8>(std::floor((b + a_other_colour.b) * 0.5f)),
            static_cast<uint8>(std::floor((a + a_other_colour.a) * 0.5f))
        );
    }

    const ColourRGBA ColourRGBA::Black(0, 0, 0);
    const ColourRGBA ColourRGBA::DarkRed(128, 0, 0);
    const ColourRGBA ColourRGBA::Red(255, 0, 0);
    const ColourRGBA ColourRGBA::DarkGreen(0, 128, 0);
    const ColourRGBA ColourRGBA::Green(0, 255, 0);
    const ColourRGBA ColourRGBA::DarkBlue(0, 0, 128);
    const ColourRGBA ColourRGBA::Blue(0, 0, 255);
    const ColourRGBA ColourRGBA::Yellow(255, 255, 0);
    const ColourRGBA ColourRGBA::Purple(255, 0, 255);
    const ColourRGBA ColourRGBA::Cyan(0, 255, 255);
    const ColourRGBA ColourRGBA::Orange(255, 128, 0);
    const ColourRGBA ColourRGBA::DarkOrange(128, 64, 0);
    const ColourRGBA ColourRGBA::Grey(128, 128, 128);
    const ColourRGBA ColourRGBA::DarkGrey(64, 64, 64);
    const ColourRGBA ColourRGBA::LightGrey(192, 192, 192);
    const ColourRGBA ColourRGBA::White(255, 255, 255);

    ColourRGBA MixColours(ColourRGBA const& a_colour_one, ColourRGBA const& a_colour_two)
    {
        return a_colour_one.Mix(a_colour_two);
    }

    ColourRGBA RandomColour()
    {
        ColourRGBA colour(0, 0, 0, 255);
        colour.RandomizeRGB();
        return colour;
    }

    ColourRGBA RandomColourRGBA()
    {
        ColourRGBA colour;
        colour.RandomizeRGBA();
        return colour;
    }

} // End of namespace ~ DFW.

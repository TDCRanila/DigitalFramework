#include <DFW/Utility/ColourUtility.h>

#include <DFW/Utility/RandomUtility.h>

#include <DFW/Defines/MathDefines.h>

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

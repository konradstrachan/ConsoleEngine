#include "pch.h"

#include "ConsoleEngine.h"
#include "ConsoleOutput.h"
#include "Utils.h"

namespace
{
    static const uint32_t blockWidth = 80;
    static const uint32_t blockHeight = 30;

    static const std::vector<wchar_t> distance = { 0x2591, 0x2592, 0x2593, 0x2588, ' '};
    static const auto maxElements = distance.size();
}

void UpdateScreen(SCE::ConsoleOutput& co)
{
    for(uint8_t y = 0; y < blockHeight ; ++y)
    {
        SCE::ConsoleOutput::GlyphAttrib ga
        { 
            GetRandomBool(),
            GetRandomBool(),
            GetRandomBool(),
            GetRandomBool(),
            GetRandomBool(),
            GetRandomBool(),
            GetRandomBool(),
            GetRandomBool()
        };

        for(uint8_t x = 0; x < blockWidth ; ++x)
        {
            wchar_t updatedChar = distance[GetRandomValue<std::vector<wchar_t>::size_type>(0, maxElements - 1)];

            co.UpdateGlyph(x, y, updatedChar, ga);
        }
    }
}

int main()
{
    const SCE::EngineParams::OnLogicTickFn tickFn = [](SCE::ConsoleOutput& co)
    {
        UpdateScreen(co);
    };

    const SCE::EngineParams::OnKeyEventFn keyEventFn = [](const KeyboardStates& ks)
    {
        (void)ks;
    };

    SCE::EngineParams params{
            true,
            blockWidth,
            blockHeight,
            10,
            tickFn,
            keyEventFn,
            {
                0x57, // W
                0x41, // A
                0x53, // S
                0x44, // D
                0x1B, // ESC
            }
        };

    StartEventLoop(params);
    
}

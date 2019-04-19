#include "pch.h"

#include "ConsoleOutput.h"
#include "InputEvents.h"
#include "Utils.h"

namespace
{
    static const uint32_t blockWidth = 80;
    static const uint32_t blockHeight = 30;

    static const std::vector<wchar_t> distance = { 0x2591, 0x2592, 0x2593, 0x2588, ' '};
    static const uint32_t maxElements = distance.size();

    struct GlyphAttrib
    {
        bool foregroundRed{false};
        bool foregroundGreen{false};
        bool foregroundBlue{false};
        bool foregroundIntense{false};

        bool backgroundRed{false};
        bool backgroundGreen{false};
        bool backgroundBlue{false};
        bool backgroundIntense{false};

        uint8_t ToAttrib() const
        {
            // Attribute values taken from https://docs.microsoft.com/en-us/windows/console/char-info-str
            return  ( foregroundRed ? FOREGROUND_RED : 0 )
                | ( foregroundGreen ? FOREGROUND_GREEN : 0 )
                | ( foregroundBlue ? FOREGROUND_BLUE : 0 )
                | ( foregroundIntense ? FOREGROUND_INTENSITY : 0 )
                | ( backgroundIntense ? BACKGROUND_INTENSITY : 0 )
                | ( backgroundRed ? BACKGROUND_RED : 0 )
                | ( backgroundGreen ? BACKGROUND_GREEN : 0 )
                | ( backgroundBlue ? BACKGROUND_BLUE : 0 )
                | ( backgroundIntense ? BACKGROUND_INTENSITY : 0 );
        }
    };
}

void UpdateScreen(ConsoleOutput& co)
{
    for(uint8_t y = 0; y < blockHeight ; ++y)
    {
        GlyphAttrib ga
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
            wchar_t updatedChar = distance[GetRandomValue<uint32_t>(0, maxElements - 1)];

            co.UpdateGlyph(x, y, updatedChar, ga.ToAttrib());
        }
    }
}

int main()
{
    // Keys checked, virtual key codes can be found :
    // https://docs.microsoft.com/en-us/windows/desktop/inputdev/virtual-key-codes
    KeyboardStates ks = { 
        {0x57, KeyState::Up},      // W
        {0x41, KeyState::Up},      // A
        {0x53, KeyState::Up},      // S
        {0x44, KeyState::Up},      // D
        {0x1B, KeyState::Up}};     // ESC

    ConsoleOutput co(blockWidth, blockHeight);

    using ClockType = std::chrono::high_resolution_clock;
    while(1)
    {       
        //
        // Logic
        //

        // Wait states
        
        // TODO other events
        while(!UpdateKeyStates(ks))
        {
            // do nothing
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(5ms);
        }

        const ClockType::time_point t1 = ClockType::now();
        
        // Test key events

        if(ks[0x1B] != KeyState::Up)
        {
            // Quit on ESC
            break;
        }

        // Update screen display

        UpdateScreen(co);

        //
        // Render
        //

        co.UpdateConsole();

        const ClockType::time_point t2 = ClockType::now();
        
        // Show the stats
        const uint64_t timeTakenToDraw = (t2 - t1).count();
        // Clock is given in nanoseconds, convert from 1^10-9
        const double approxFPS = 1 / (timeTakenToDraw / 1000000000.0);

        co.SetConsolePos(0, blockHeight+1);
        std::wcout << "                               ";
        co.SetConsolePos(0, blockHeight+1);
        std::wcout << "FPS: " << approxFPS << std::flush;
    }

    return EXIT_SUCCESS;
}

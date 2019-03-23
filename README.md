# Console Output Experiments

This project started as a simple experiment to determine what graphical output could be accomplished using just unicode characters on a standard Windows terminal. The idea was to determine if a console could be used to create something that could form the basis of an extremely lightweight rendering back-end for visualising output.

Initially the output speed to the console of a small viewport was painfully slow. The code was written with platform portability in mind and as such I used only methods from the C++ standard to write to the console.

The first attempt was to use streams via std::cout as shown below:

    void PaintScreenStreams()
    {
        SetConsolePos(0, 0);
    
        for(uint32_t y = 0; y < blockHeight ; ++y)
        {
            for(uint32_t x = 0; x < blockWidth ; ++x)
            {
                std::wcout << distance[GetRandomValue<uint32_t>(0, maxElements - 1)];
            }
    
            std::wcout << '\n';
        }
    }

This resulted in uninspiring **~6 FPS** with visible horizontal refreshing. Not idea for any use case that needs to update the screen quickly.

My next attempt attempted to avoid the buffering of streams and writing character by character by using a printf style function in a line by line approach.

    void PaintScreenPrintf()
    {
        SetConsolePos(0, 0);
    
        for( uint32_t y = 0; y < blockHeight ; ++y)
        {
            std::wstring lineBuffer;
            for( uint32_t x = 0; x < blockWidth ; ++x)
            {
                lineBuffer += distance[GetRandomValue<uint32_t>(0, maxElements - 1)];
            }
            lineBuffer += L"\n";
    
            wprintf(lineBuffer.c_str());
        }
    }

The result was better with between **9 and 10 FPS** achieved. Despite couple to doubling the draw speed it still didn't feel particularly useful. I then discarded standard library functions altogether looking to see if I could find a Windows API call that would allow me to directly access the underlying text buffer of the console window. My aim was to create a double buffering mechanism so that even with a low refresh rate, screen updates would seem atomic to someone looking at the console. After a bit of digging I found the WriteConsoleOutput Windows API function.

    void PaintScreenBuffer()
    {
        CHAR_INFO buffer[blockWidth * blockHeight];
    
        const COORD startDest = {0, 0};
        const COORD size = {blockWidth, blockHeight};
        SMALL_RECT rect = {0, 0, blockWidth, blockHeight};
    
        uint32_t offset = 0;
        for(uint32_t y = 0; y < blockHeight ; ++y)
        {
            for(uint32_t x = 0; x < blockWidth ; ++x)
            {
                uint32_t offset = x + (y * blockWidth);
                buffer[offset].Char.UnicodeChar = distance[GetRandomValue<uint32_t>(0, maxElements - 1)];
                // Attribute values taken from https://docs.microsoft.com/en-us/windows/console/char-info-str
                buffer[offset].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            }
        }
    
        // https://docs.microsoft.com/en-us/windows/console/writeconsoleoutput
        WriteConsoleOutput( outputHandle, buffer, size, startDest, &rect );
    
        // Set console position to the line afte we've written to follow other behaviour
        SetConsolePos(0, static_cast<uint8_t>(blockHeight + 1));
    }

Despite requiring a little more wrangling than the previous approaches the result was extraordinary. On an i7-3770k this code resulted in **8000 FPS**! It seems extraordinary that the previous approaches were so slow by comparison.

### Notes

Testing was performed using Release mode with full optimisations executed from outside the debugger. In addition - a special console unicode mode was set by calling setmode to enable the display of unicode characters. This code unfortunately is extremely Windows coupled given the number of platform specific calls, however as I build on this hopefully I will generalise it without losing performance.

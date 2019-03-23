#include "pch.h"
#include <iostream>
#include <vector>
#include <chrono>

#include <fcntl.h>
#include <io.h>
#include <Windows.h>

namespace
{
    static const uint32_t blockWidth = 80;
    static const uint32_t blockHeight = 30;

    static const std::vector<wchar_t> distance = { 0x2591, 0x2592, 0x2593, 0x2588, ' '};
    static const uint32_t maxElements = distance.size();

    static const HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
}

template<typename T>
inline T GetRandomValue( T nMin, T nMax )
{
    static_assert(std::is_integral<T>::value, "Unsupported tpye");

    if( nMax == 0 )
    {
        _ASSERT_EXPR( 0, L"Random value with zero?" );
        return  0;
    }

    _ASSERT_EXPR( nMin <= nMax, L"Min Max issue" );

    T nResult = ( ( rand() % ( (nMax + 1) - nMin ) ) + nMin );
    _ASSERT( nResult >= nMin );
    _ASSERT( nResult <= nMax );

    return nResult;
}

void SetConsolePos(uint8_t x, uint8_t y)
{
    SetConsoleCursorPosition(outputHandle, {x, y});
}

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

int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);

    std::vector<wchar_t> distance = { 0x2591, 0x2592, 0x2593, 0x2588, 0x25E8 };

    using ClockType = std::chrono::high_resolution_clock;
    while(1)
    {
        const ClockType::time_point t1 = ClockType::now();

        // Abuse the console window (pick one)
#if 0
        PaintScreenStreams();
#endif
#if 0
        PaintScreenPrintf();
#endif
#if 0
        PaintScreenBuffer();
#endif
        
        const ClockType::time_point t2 = ClockType::now();

        // Show the stats
        const uint64_t timeTakenToDraw = (t2 - t1).count();
        // Clock is given in nanoseconds, convert from 1^10-9
        const double  approxFPS = 1 / (timeTakenToDraw / 1000000000.0);

        std::wcout << "FPS: " << approxFPS << std::flush;
    }

    return EXIT_SUCCESS;
}

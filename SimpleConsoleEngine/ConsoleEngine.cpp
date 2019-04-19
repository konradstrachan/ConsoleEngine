#include "pch.h"
#include "ConsoleEngine.h"
#include "ConsoleOutput.h"

namespace SCE
{

bool StartEventLoop(const EngineParams& params)
{
    const EngineParams ep = params;

    // Keys checked, virtual key codes can be found :
    // https://docs.microsoft.com/en-us/windows/desktop/inputdev/virtual-key-codes
    KeyboardStates ks;
    
    for( const auto& kc : ep.keyCodesToMonitor )
    {
        ks[kc] = KeyState::Up;
    }
   
    ConsoleOutput co(params.viewWidthGlyphs, params.viewHeightGlyphs);

    const std::chrono::seconds timeBetweenFPSUpdates = std::chrono::seconds(1);
    uint64_t frameTimes{ 0 };
    uint32_t frames{ 0 };

    using ClockType = std::chrono::high_resolution_clock;

    ClockType::time_point tLastFPSUpdate = ClockType::now();
    ClockType::time_point t1 = ClockType::now();

    while(1)
    {
        //
        // Logic
        //

        UpdateKeyStates(ks);

        if(ep.keyStateChangeFn)
        {
            (*ep.keyStateChangeFn)(ks);
        }

        // do nothing - note this will just block without
        // paying attention to any events that may be raised
        std::this_thread::sleep_for(
            std::chrono::milliseconds(
                ep.numberOfMilliSecondsBetweenTicks));

        if(ks[0x1B] != KeyState::Up)
        {
            // Quit on ESC
            break;
        }

        if(ep.shouldShowFPS)
        {
            t1 = ClockType::now();
        }

        //
        // Tick
        //

        if(ep.logicFn)
        {
            (*ep.logicFn)(co);
        }

        //
        // Render
        //

        // TODO only update if something has changed
        co.UpdateConsole();

        if(ep.shouldShowFPS)
        {
            const ClockType::time_point t2 = ClockType::now();

            // Show the stats
            const uint64_t timeTakenToDraw = (t2 - t1).count();
            frameTimes += timeTakenToDraw;
            ++frames;

            if( (ClockType::now() - tLastFPSUpdate) >= timeBetweenFPSUpdates )
            {
                tLastFPSUpdate = ClockType::now();
                // Clock is given in nanoseconds, convert from 1^10-9
                const auto averageFrameTime = (frameTimes / static_cast<double>(frames));
                const double approxFPS = (1 / ( averageFrameTime / 1000000000.0) );
                frameTimes = 0;
                frames = 0;

                co.SetConsolePos(0, ep.viewHeightGlyphs + 1 );
                std::wcout << "                               ";
                co.SetConsolePos(0, ep.viewHeightGlyphs + 1);
                std::wcout << "FPS: " << approxFPS << std::flush;
            }
        }
    }

    return true;
}

}   // namespace SCE
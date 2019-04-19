#pragma once

#include "InputEvents.h"
#include "ConsoleOutput.h"
#include "Utils.h"

namespace SCE
{

struct EngineParams
{
    typedef std::function<void (ConsoleOutput&)> OnLogicTickFn;
    typedef std::function<void (const KeyboardStates&)> OnKeyEventFn;

    const bool shouldShowFPS;
    const ConsoleOutput::ConsoleBoundType viewWidthGlyphs;
    const ConsoleOutput::ConsoleBoundType viewHeightGlyphs;
    const uint32_t numberOfMilliSecondsBetweenTicks;
    const std::optional<OnLogicTickFn> logicFn;
    const std::optional<OnKeyEventFn> keyStateChangeFn;
    std::vector<uint32_t> keyCodesToMonitor;
};

bool StartEventLoop(const EngineParams& params);

}   // namespace SCE
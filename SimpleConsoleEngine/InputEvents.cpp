#include "pch.h"
#include "InputEvents.h"

bool UpdateKeyStates(KeyboardStates& states)
{
    bool raiseEvent{false};

    // Handle Keyboard Input
    for (auto& [key, state] : states)
    {
        const uint16_t stateNow = GetAsyncKeyState(key);
        // According to :
        // https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-getasynckeystate
        // 'if most significant bit is set, the key is down'
        KeyState newState = static_cast<KeyState>((stateNow & 0x8000) != 0);

        if(state == KeyState::Down && newState == KeyState::Down)
        {
            state = KeyState::Held;
            raiseEvent = true;
        }
        else if(state == KeyState::Held && newState == KeyState::Down)
        {
            raiseEvent = true;
        }
        else if(state != newState)
        {
            state = newState;
            raiseEvent = true;
        }
    }

    return raiseEvent;
}
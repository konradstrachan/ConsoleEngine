#pragma once

enum class KeyState : uint8_t
{
    Up = 0,
    Down = 1,
    Held = 2
};

typedef std::map<uint32_t, KeyState> KeyboardStates;

bool UpdateKeyStates(KeyboardStates& states);
#pragma once

class ConsoleOutput
{
public:
    using ConsoleBoundType = uint8_t;

    ConsoleOutput(ConsoleBoundType boundX, ConsoleBoundType boundY);

    bool UpdateGlyph(ConsoleBoundType x, ConsoleBoundType y, std::optional<wchar_t> character, std::optional<uint8_t> attributes);

    void UpdateConsole();

    void SetConsolePos(ConsoleBoundType x, ConsoleBoundType y);

private:

    const ConsoleBoundType m_boundX;
    const ConsoleBoundType m_boundY;
    std::vector<CHAR_INFO> m_buffer;

    HANDLE m_outputHandle;
};
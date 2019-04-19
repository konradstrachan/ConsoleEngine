#include "pch.h"
#include "ConsoleOutput.h"

namespace SCE
{

ConsoleOutput::ConsoleOutput(ConsoleBoundType boundX, ConsoleBoundType boundY)
    : m_boundX(boundX)
    , m_boundY(boundY)
{
    m_buffer.resize(boundX * boundY);
    m_outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    for(uint32_t i = 0 ; i < m_buffer.size() ; ++i)
    {
        m_buffer[i].Char.UnicodeChar = L' ';
        m_buffer[i].Attributes = 0;
    }

    _setmode(_fileno(stdout), _O_U16TEXT);
}

bool ConsoleOutput::UpdateGlyph(ConsoleBoundType x, ConsoleBoundType y, std::optional<wchar_t> character, std::optional<GlyphAttrib> attributes)
{
    if(x >= m_boundX || y >= m_boundY)
    {
        _ASSERT(false);
        return false;
    }

    if( !character && !attributes)
    {
        _ASSERT(false);
        return false;
    }

    CHAR_INFO& element = m_buffer[x + (y * m_boundX)];

    if(character)
    {
        element.Char.UnicodeChar = *character;
    }

    if(attributes)
    {
        element.Attributes = attributes->ToAttrib();
    }

    return true;
}

void ConsoleOutput::UpdateConsole()
{
    const COORD startDest = {0, 0};
    const COORD size = {m_boundX, m_boundY};
    SMALL_RECT rect = {0, 0, m_boundX, m_boundY};

    // https://docs.microsoft.com/en-us/windows/console/writeconsoleoutput
    WriteConsoleOutput( m_outputHandle, &m_buffer[0], size, startDest, &rect );    
}

void ConsoleOutput::SetConsolePos(ConsoleBoundType x, ConsoleBoundType y)
{
    SetConsoleCursorPosition(m_outputHandle, {x, y});
}

}   // namespace SCE
#pragma once

namespace SCE
{

class ConsoleOutput
{
public:
    using ConsoleBoundType = uint8_t;

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

    ConsoleOutput(ConsoleBoundType boundX, ConsoleBoundType boundY);

    bool UpdateGlyph(ConsoleBoundType x, ConsoleBoundType y, std::optional<wchar_t> character, std::optional<GlyphAttrib> attributes);

    void UpdateConsole();

    void SetConsolePos(ConsoleBoundType x, ConsoleBoundType y);

private:

    const ConsoleBoundType m_boundX;
    const ConsoleBoundType m_boundY;
    std::vector<CHAR_INFO> m_buffer;

    HANDLE m_outputHandle;
};

}   // namespce SCE
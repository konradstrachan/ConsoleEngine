#pragma once

template<typename T>
inline T GetRandomValue( T nMin, T nMax )
{
    static_assert(std::is_integral<T>::value, "Unsupported type");

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

inline bool GetRandomBool()
{
    return GetRandomValue<uint32_t>( 0, 1 ) == 1;
}
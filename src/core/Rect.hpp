#pragma once

namespace ari::core
{
    template<typename T>
    struct Rect
    {
        T   x = 0,
            y = 0,
            width = 0,
            height = 0;
    };
    
    typedef Rect<int> RectI;
    
} // namespace ari::core

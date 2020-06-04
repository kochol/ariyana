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

        Rect<T> operator * (float s)
        {
            return { static_cast<T>(float(x) * s), 
                static_cast<T>(float(y) * s), 
                static_cast<T>(float(width) * s), 
                static_cast<T>(float(height) * s) };
        }

    };
    
    typedef Rect<int> RectI;

    typedef Rect<float> RectF;
    
} // namespace ari::core

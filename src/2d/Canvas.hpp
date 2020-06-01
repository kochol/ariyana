#pragma once

#include "Node2D.hpp"
#include "core/Rect.hpp"

namespace ari::en
{
    class Camera2D;

    struct Canvas : public Node2D
    {
        ARI_COMPONENT_CHILD(Canvas, Node2D)

        core::RectI Rect;                   //!< The canvas position

        void UpdateRect();

        void UpdateCamera(Camera2D* _camera, int _frameTurnIndex);

    private:

		core::RectI _RealRect;              //!< This value updates by engine     

    };
    
} // namespace ari::en

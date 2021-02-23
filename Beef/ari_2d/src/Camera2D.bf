using System;
using ari.math;

namespace ari.en
{
	[Component("Camera2D", "Node2D")]
	public class Camera2D : Node2D
	{
		protected float4x4 proj;

		public this()
		{
			is_renderable = true;
		}

		public void MoveLR(in float _speed)
		{
			Position.x += _speed;
		}

		public void MoveUD(in float _speed)
		{
			Position.y += _speed;
		}

		public override void Render(in int _frameTurnIndex)
		{
			RectI rect;
			Gfx.GetViewportSize(out rect);
			proj = float4x4.Ortho(rect.width, rect.height, 0.1f, 100, 0, false);
			Gfx.SetViewProjMatrix(final_mat[_frameTurnIndex], proj);
		}
	}
}

using System;
using ari.math;

namespace ari.en
{
	[Component("Sprite2D", "Node2D")]
	public class Sprite2D : Node2D
	{
		public TextureHandle Texture = .();
		public Color		 Color = .WHITE;
		public float4		 UV = .(0, 0, 1, 1);

		public this()
		{
			is_renderable = true;
		}

		public override void Render(in int _frameTurnIndex)
		{
			
		}
	}
}

using System;
using ari.math;

namespace ari.en
{
	[Component("Node2D")]
	public class Node2D : Node, IComponent
	{
		public float2 Position;
		public float  Rotation;
		public float2 Scale = .(1, 1);

		public virtual void Render(in int _frameTurnIndex) { }

		protected float4x4[3] final_mat;
		protected bool is_renderable = false;
	}
}

using System;
using System.Collections;

namespace ari.en
{
	public class AriSystem
	{
		List<AriSystem> m_children = null ~ delete _;

		// World call this on configure
		protected virtual void Configure(World _world) { }

		protected virtual void Unconfigure(World _world) { }

		protected virtual void Update(World _world, float _elapsed) { }

		public void AddChild(World world, AriSystem system)
		{
			if (m_children == null)
				m_children = new List<AriSystem>();
			m_children.Add(system);

			system.Configure(world);
		}
	}
}

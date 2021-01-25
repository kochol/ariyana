using System;
using System.Collections;

namespace ari.en
{
	[Component("Node")]
	public class Node 
	{
		String name = null ~ delete _;
		public StringView Name {
			get => name;
			set {
				if (name == null)
					name = new String();
				name.Set(value);
			}
		}

		Dictionary<uint32, List<Node>> children = null ~ DeleteDictionaryAndValues!(_);
		Dictionary<uint32, List<Node>> children_base = null ~ DeleteDictionaryAndValues!(_);

		public this()
		{
		}

		public void AddChild(Node _child)
		{
			if (children == null)
			{
				children = new Dictionary<uint32, List<Node>>();
			}
			let id = _child.GetId();
			if (!children.ContainsKey(_child.GetId()))
				children.Add(id, new List<Node>());
			children[id].Add(_child);

			let base_id = _child.GetBaseId();
			if (id != base_id)
			{
				if (!children_base.ContainsKey(base_id))
					children_base.Add(base_id, new List<Node>());
				children_base[base_id].Add(_child);
			}
		}

		public void RemoveChild(Node _child)
		{
			let id = _child.GetId();
			children[id].Remove(_child);
			let base_id = _child.GetBaseId();
			if (id != base_id)
			{
				children_base[base_id].Remove(_child);
			}
		}
	}
}

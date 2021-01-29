using System;
using System.Collections;

namespace ari.en
{
	[Component("Node")]
	public class Node : IComponent
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
		Node parent = null;

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

			_child.parent = this;
		}

		public void RemoveChild(Node _child)
		{
			let id = _child.GetId();
			bool removed = children[id].Remove(_child);
			let base_id = _child.GetBaseId();
			if (id != base_id)
			{
				removed = children_base[base_id].Remove(_child);
			}
			if (removed)
				_child.parent = null;
		}

		public bool HasChildWithId(uint32 _id)
		{
			if (children_base.ContainsKey(_id) && children_base[_id].Count > 0)
				return true;
			if (children.ContainsKey(_id) && children[_id].Count > 0)
				return true;
			return false;
		}

		public List<Node> GetChildren(uint32 _id)
		{
			if (children_base.ContainsKey(_id) && children_base[_id].Count > 0)
				return children_base[_id];
			if (children.ContainsKey(_id) && children[_id].Count > 0)
				return children[_id];
			return null;
		}
	}
}

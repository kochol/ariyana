using System;

namespace ari
{
	public class Node
	{
		void* _obj;

		String _name;

		[CLink]
		static extern void SetNodeName(void* _obj, char8* _name);

		public String Name
		{
			get
			{
				return _name;
			}
			set
			{
				Runtime.Assert(_obj != null);
				_name = value;
				SetNodeName(_obj, _name);
			}
		}

		[CLink]
		static extern char8* GetNodeName(void* _obj);

		protected this(void* obj)
		{
			_obj = obj;
			_name = new String(GetNodeName(_obj));
		}

		public ~this()
		{
			delete _name;
		}

		[CLink]
		static extern void NodeAddChild(void* _node, void* _child);

		public void AddChild(Node _child)
		{
			NodeAddChild(_obj, _child._obj);
		}

		[CLink]
		static extern void NodeRemoveChild(void* _node, void* _child);

		public void RemoveChild(Node _child)
		{
			NodeRemoveChild(_obj, _child._obj);
		}

	}
}

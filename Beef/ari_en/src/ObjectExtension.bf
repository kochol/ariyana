using System;
using System.Diagnostics;

namespace ari.en
{
	static
	{
		public static uint32 GetId(this Object obj)
		{
			if (let c = obj.GetType().GetCustomAttribute<ComponentAttribute>())
				return c.Id;
			else
			{
				var str = scope String();
				obj.GetType().GetName(str);
				var err = scope String();
				err.AppendF("Add Component attribute to {} class/struct", str);
				Debug.FatalError(err);
			}
			return 0;
		}

		public static uint32 GetBaseId(this Object obj)
		{
			if (let c = obj.GetType().GetCustomAttribute<ComponentAttribute>())
				return c.BaseId;
			else
			{
				var str = scope String();
				obj.GetType().GetName(str);
				var err = scope String();
				err.AppendF("Add Component attribute to {} class/struct", str);
				Debug.FatalError(err);
			}
			return 0;
		}
	}
}

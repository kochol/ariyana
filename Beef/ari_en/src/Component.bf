using System;
using System.Diagnostics;

namespace ari.en
{
	abstract class Component<T>
	{
		public static uint32 id = 0;
		public static uint32 Id
		{
			get {
				if (id == 0)
				{
					if (let c = typeof(T).GetCustomAttribute<ComponentAttribute>())
					{
						id = c.Id;
						base_id = c.BaseId;
					}
					else
					{
						var str = scope String();
						typeof(T).GetName(str);
						str.AppendF("Add Component attribute to {}", str);
						Debug.FatalError(str);
					}
				}
				return id;
			}
		}

		public static uint32 base_id = 0;
		public static uint32 BaseId
		{
			get {
				if (base_id == 0)
				{
					if (let c = typeof(T).GetCustomAttribute<ComponentAttribute>())
					{
						id = c.Id;
						base_id = c.BaseId;
					}
					else
					{
						var str = scope String();
						typeof(T).GetName(str);
						str.AppendF("Add Component attribute to {}", str);
						Debug.FatalError(str);
					}
				}
				return base_id;
			}
		}
	}
}

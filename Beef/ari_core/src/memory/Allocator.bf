namespace ari.core
{
	abstract class Allocator
	{
		protected int m_totalSize;
		protected int m_used;
		protected int m_peak;

		public this(int totalSize)
		{
			m_totalSize = totalSize;
		}

		public ~this()
		{
			m_totalSize = 0;
		}

		public abstract void* Allocate(int size, int alignment = 0);

		public abstract void Free(void* ptr);

		public abstract void Init();

		protected static int CalculatePadding(int baseAddress, int alignment)
		{
			int multiplier = (baseAddress / alignment) + 1;
			int alignedAddress = multiplier * alignment;
			return alignedAddress - baseAddress;
		}

		protected static int CalculatePaddingWithHeader(int baseAddress, int alignment, int headerSize)
		{
			int padding = CalculatePadding(baseAddress, alignment);
			int neededSpace = headerSize;

			if (padding < neededSpace)
			{
				// Header does not fit - Calculate next aligned address that header fits
				neededSpace -= padding;

				// How many alignments I need to fit the header        
		    	if(neededSpace % alignment > 0)
				{
			        padding += alignment * (1+(neededSpace / alignment));
		    	}
				else
				{
			        padding += alignment * (neededSpace / alignment);
		    	}
			}

			return padding;
		}
	}
}

#include "net.hpp"

namespace ari::net
{
	//------------------------------------------------------------------------------
	bool InitNetwork()
	{
		return InitializeYojimbo();
	}

	//------------------------------------------------------------------------------
	void ShutdownNetwork()
	{
		ShutdownYojimbo();
	}

}
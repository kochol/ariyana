#include "Platform.hpp"
#include <windows.h>
#include <IPTypes.h>
#include <iphlpapi.h>
#include <winerror.h>

namespace ari::io
{
	core::String GetMacAddress()
	{
		static core::String Result;
		if (Result.Length() > 0)
			return Result;

		IP_ADAPTER_INFO IpAddresses[16];
		ULONG OutBufferLength = sizeof(IP_ADAPTER_INFO) * 16;

		// Read the adapters
		ULONG RetVal = GetAdaptersInfo(IpAddresses, &OutBufferLength);
		if (RetVal == NO_ERROR)
		{
			PIP_ADAPTER_INFO AdapterList = IpAddresses;
			// Walk the set of addresses copying each one
			while (AdapterList)
			{
				// If there is an address to read
				if (AdapterList->AddressLength > 0)
				{
					// Copy the data and say we did
					Result.Assign((char*)AdapterList->Address, 0, AdapterList->AddressLength);
					break;
				}
				AdapterList = AdapterList->Next;
			}
		}
		return Result;
	}

} // namespace ari::io

#include "Platform.hpp"
#include "sx/string.h"
#include <sys/sysinfo.h>
#include <sched.h>
#include <sys/vfs.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <stdio.h>
#include <string.h>

namespace ari::io
{
    core::String GetMacAddress()
    {
       	struct ifaddrs *ifap, *ifaptr;
        static core::String Result;
        if (Result.Length() > 0)
			return Result;

        if (getifaddrs(&ifap) == 0)
        {
            for (ifaptr = ifap; ifaptr != nullptr; ifaptr = (ifaptr)->ifa_next)
            {
                struct ifreq ifr;

                strncpy(ifr.ifr_name, ifaptr->ifa_name, IFNAMSIZ-1);

                int Socket = socket(AF_UNIX, SOCK_DGRAM, 0);
                if (Socket == -1)
                {
                    continue;
                }

                if (ioctl(Socket, SIOCGIFHWADDR, &ifr) == -1)
                {
                    close(Socket);
                    continue;
                }

                close(Socket);

                if (ifr.ifr_hwaddr.sa_family != ARPHRD_ETHER)
                {
                    continue;
                }

                const uint8_t *MAC = (uint8_t*) ifr.ifr_hwaddr.sa_data;
                char str[256];
                sx_snprintf(str, 256, "%d-%d-%d-%d-%d-%d", MAC[0], MAC[1], MAC[2], MAC[3], MAC[4], MAC[5]);
                Result = str;

                break;
            }

            freeifaddrs(ifap);
        }

        return Result;
    }
    
} // namespace ari::io

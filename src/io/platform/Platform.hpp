#include "core/string/String.hpp"

namespace ari::io
{
    /**
     * @brief Get the Mac Address string
     * On android it returns device id
     * @return core::String 
     */
    core::String GetMacAddress();

    /**
     * @brief Get the unique Device ID.
     * This function works on android and windows for now
     * On windows it hash the mac address of network adapter
     * On Android it hash the android device ID so it can change if user factory reset her/his device.
     * @return String 
     */
    uint32_t GetDeviceID();

} // namespace ari::io

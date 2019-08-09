#pragma once
#include "yojimbo.h"

namespace ari::net
{
	bool InitNetwork();

	void ShutdownNetwork();

    /*// Server functions
    bool ServerSetup();
    void ServerUpdate();

    // Client
    bool ClientConnect();
    void ClientUpdate();
    */   
   
	// a simple test message
	enum class GameMessageType {
		TEST,
		COUNT
	};

	// two channels, one for each type that Yojimbo supports
	enum class GameChannel {
		RELIABLE,
		UNRELIABLE,
		COUNT
	};

	// the client and server config
	struct GameConnectionConfig : yojimbo::ClientServerConfig {
		GameConnectionConfig() {
			numChannels = 2;
			channel[(int)GameChannel::RELIABLE].type = yojimbo::CHANNEL_TYPE_RELIABLE_ORDERED;
			channel[(int)GameChannel::UNRELIABLE].type = yojimbo::CHANNEL_TYPE_UNRELIABLE_UNORDERED;
		}
	};	

} // namespace ari::net

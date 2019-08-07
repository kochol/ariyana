#include "GameAdapter.hpp"
#include "ServerSystem.hpp"

namespace ari::net
{
    void GameAdapter::OnServerClientConnected(int clientIndex)
    {
        if (m_server != NULL)
        {
            m_server->ClientConnected(clientIndex);
        }
    }

    void GameAdapter::OnServerClientDisconnected(int clientIndex)
    {
        if (m_server != NULL)
        {
            m_server->ClientDisconnected(clientIndex);
        }
    }
} // namespace ari::net

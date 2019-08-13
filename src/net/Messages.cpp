#include "Messages.hpp"
#include "ClientSystem.hpp"

namespace ari::net
{
	//------------------------------------------------------------------------------
	en::EntityHandle CreateEntityMessage::GetEntity(const uint32_t& server_entity_handle)
	{
		return client_system->GetEntity(server_entity_handle);
	}

	//------------------------------------------------------------------------------
	void CreateEntityMessage::AddEntity(const uint32_t& server_entity_handle, const uint32_t& client_entity_handle)
	{
		client_system->AddEntity(server_entity_handle, client_entity_handle);
	}

} // namespace ari::net

#pragma once

#include "core/containers/Map.hpp"
#include "core/containers/Array.hpp"
#include "World.hpp"
#include "ComponentHandle.hpp"
#include "core/defines.hpp"

namespace ari
{
	namespace en
	{
		class Node
		{			
		public:

			ARI_COMPONENT(Node)

			virtual ~Node() = default;

			void AddChild(Node* _child);

			void RemoveChild(Node* _child);

			bool HasChildWithId(uint32_t _id) const;

			const core::Array<Node*>& GetChildren(uint32_t _id);

			Node* GetChild(uint32_t _id);

		private:

			core::Map<uint32_t, core::Array<Node*>>	m_mChilds;

		};
		
	} // en
	
} // ari

#pragma once

#include "core/containers/Map.hpp"
#include "core/containers/Array.hpp"
#include "World.hpp"
#include "ComponentHandle.hpp"
#include "core/defines.hpp"
#include "core/string/String.hpp"

namespace ari
{
	namespace en
	{
		class Node
		{			
		public:

			core::String Name;

			ARI_COMPONENT(Node)

			virtual ~Node() = default;

			void AddChild(Node* _child);

			void RemoveChild(Node* _child);

			bool HasChildWithId(uint32_t _id) const;

			const core::Array<Node*>& GetChildren(uint32_t _id);

			core::Array<Node*> GetChildrenContains(const core::String& _name);

			template<typename FUNC>
			void GetChildren(FUNC _func)
			{
				for(const auto& l: m_mChilds)
				{
					for(auto n: l.value)
					{
						_func(n);
					}
				}
			}

			Node* GetNodeWithName(const core::String& _name);

			Node* GetChild(uint32_t _id);

			Node* GetParent() const { return m_pParent; }

		protected:

			core::Map<uint32_t, core::Array<Node*>>	m_mChilds;
			Node* m_pParent = nullptr;

		};
		
	} // en
	
} // ari

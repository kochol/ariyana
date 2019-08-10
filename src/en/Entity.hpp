#pragma once
#include "core/defines.hpp"

namespace ari
{
	namespace en
	{
		ARI_HANDLE(EntityHandle)
			
		struct Entity
        {
			EntityHandle dummy1;
			EntityHandle dummy2;

			//! Should network replicates this entity?
			bool bReplicates = false;
        };        

	} // en
	
} // ari

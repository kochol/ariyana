#pragma once

#include "core/defines.hpp"
#include "en/Node.hpp"
#include "core/HandmadeMath.h"
#include "sx/macros.h"

namespace ari::en
{
	class Node3D: public Node
	{
	public:

		ARI_COMPONENT(Node3D)

		//! Constructor
		Node3D() : _isRenderable(false)
		{
			Position = HMM_Vec3(0.0f, 0.0f, 0.0f);
			Scale = HMM_Vec3(1.0f, 1.0f, 1.0f);
			Rotation = HMM_Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
		}

		//! Destructor
		virtual ~Node3D() = default;

		//! Render
		virtual void Render(const hmm_mat4& matrix) { sx_unused(matrix); }

		hmm_vec3 Position;
		hmm_quaternion Rotation;
		hmm_vec3 Scale;

		hmm_mat4 _finalMat;
		bool _isRenderable;

	}; // Node3D
	
} // ari

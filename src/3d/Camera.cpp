#include "Camera.hpp"
#include "en/ComponentManager.hpp"
#include "en/World.hpp"

namespace ari::en
{
	ARI_COMPONENT_IMP_CHILD(Camera, Node3D);

	void Camera::Rotate(float _angle, const sx_vec3& _axis)
	{
		sx_vec3 vLook;				// Looking point.

		const sx_vec3 vDir = Target - Position; // Looking direction.

		const float cost = sx_cos(_angle);
		const float sint = sx_sin(_angle);

		// Find the new x position for the new rotated point
		vLook.x = (cost + (1 - cost) * _axis.x * _axis.x)				* vDir.x;
		vLook.x += ((1 - cost) * _axis.x * _axis.y - _axis.z * sint)	* vDir.y;
		vLook.x += ((1 - cost) * _axis.x * _axis.z + _axis.y * sint)	* vDir.z;

		// Find the new y position for the new rotated point
		vLook.y = ((1 - cost) * _axis.x * _axis.y + _axis.z * sint)		* vDir.x;
		vLook.y += (cost + (1 - cost) * _axis.y * _axis.y)				* vDir.y;
		vLook.y += ((1 - cost) * _axis.y * _axis.z - _axis.x * sint)	* vDir.z;

		// Find the new z position for the new rotated point
		vLook.z = ((1 - cost) * _axis.x * _axis.z - _axis.y * sint)		* vDir.x;
		vLook.z += ((1 - cost) * _axis.y * _axis.z + _axis.x * sint)	* vDir.y;
		vLook.z += (cost + (1 - cost) * _axis.z * _axis.z)				* vDir.z;


		// Now we just add the newly rotated vector to our position to set
		// our new rotated view of our camera.
		Target.x = Position.x + vLook.x;
		Target.y = Position.y + vLook.y;
		Target.z = Position.z + vLook.z;

	} // Rotate

	void Camera::RotateByMouse(int _x, int _y, float _speed)
	{
		if ((_x == 0) && (_y == 0))
			return;

		_x *= -1;
		_y *= -1;
		float angleY = 0.0f; 
		float angleZ = 0.0f;

		angleY = float(_x) * _speed;
		angleZ = float(_y) * _speed;

		m_fLastRotX = m_fCurRotX;

		m_fCurRotX += angleZ;

		// calculate the correct up
		sx_vec3 view = Target - Position;
		view = sx_vec3_norm(view);
		sx_vec3 right = sx_vec3_cross(Up, view);
		right = sx_vec3_norm(right);
		sx_vec3 up = sx_vec3_cross(view, right);

		sx_vec3 vAxis = sx_vec3_cross(view, up);
		vAxis = sx_vec3_norm(vAxis);

		if (m_fCurRotX > 1.0f)
		{
			m_fCurRotX = 1.0f;
			if (m_fLastRotX < 1.0f)
			{
				Rotate(1.0f - m_fLastRotX, vAxis);
			} // if (LastRotX != 1.0f)

		} // if (m_fCurRotX > 1.0f)
		else if (m_fCurRotX < -1.0f)
		{
			m_fCurRotX = -1.0f;
			if (m_fLastRotX > -1.0f)
			{
				Rotate(-1.0f - m_fLastRotX, vAxis);
			} // if (m_fLastRotX < -1.0f)

		} // if (m_fCurRotX < -1.0f)
		else
		{
			Rotate(angleZ, vAxis);
		}
		Rotate(-angleY, sx_vec3f(0.0f, 1.0f, 0.0f));
	}

	void Camera::MoveBF(const float & _speed)
	{
		sx_vec3 v = Target - Position;
		v = sx_vec3_norm(v);
		Position.x += v.x * _speed;
		Position.y += v.y * _speed;
		Position.z += v.z * _speed;

		Target.x += v.x * _speed;
		Target.y += v.y * _speed;
		Target.z += v.z * _speed;
	}

	void Camera::MoveLR(const float & _speed)
	{
		// calculate the correct up
		sx_vec3 view = Target - Position;
		view = sx_vec3_norm(view);
		sx_vec3 right = sx_vec3_cross(Up, view);
		right = sx_vec3_norm(right);

		Position.x += right.x * _speed;
		Position.y += right.y * _speed;
		Position.z += right.z * _speed;

		Target.x += right.x * _speed;
		Target.y += right.y * _speed;
		Target.z += right.z * _speed;
	}

	void Camera::MoveUD(const float & _speed)
	{
		Position.y += _speed;
		Target.y += _speed;
	}

	gfx::Viewport* Camera::GetViewport()
	{
		if (!m_bHasViewPort)
			return nullptr;
		return &m_viewport;
	}

	void Camera::SetViewport(const gfx::Viewport& _viewport)
	{
		m_bHasViewPort = true;
		m_viewport = _viewport;
	}

}

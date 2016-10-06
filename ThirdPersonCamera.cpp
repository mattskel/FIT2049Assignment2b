#include "ThirdPersonCamera.h"

ThirdPersonCamera::ThirdPersonCamera()
{
	SetFollowTarget(NULL, Vector3(0.0f, 10.0f, -10.0f));
}

ThirdPersonCamera::ThirdPersonCamera(GameObject* target, Vector3 offset)
{
	SetFollowTarget(target, offset);
}

void ThirdPersonCamera::Update(float timestep) {

	if (m_objectToFollow != NULL) {

		
		SetLookAt(m_objectToFollow->GetPosition());

		float heading = m_objectToFollow->GetYRotation();
		Vector3 velocity = dynamic_cast<PhysicsObject*>(m_objectToFollow)->GetVelocity();

		Matrix rotation = Matrix::CreateRotationY(heading);

		Vector3 rotatedOffset = Vector3::TransformNormal(
			m_offset,
			rotation);

		// target position in header
		// not where it is, where it wants to be

		/*m_currentPosition = Vector3::Lerp(m_currentPosition,
		m_targetPosition,
		m_moveSpeed*timestep);*/

		//SetPosition(m_objectToFollow->GetPosition() + rotatedOffset);
		SetPosition(Vector3::Lerp(GetPosition(),
			m_objectToFollow->GetPosition() + rotatedOffset,
			timestep * 3.0));
	}
	Camera::Update(timestep);
}

void ThirdPersonCamera::SetFollowTarget(GameObject* target, Vector3 offset)
{
	m_objectToFollow = target;
	m_offset = offset;
}
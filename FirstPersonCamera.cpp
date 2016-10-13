#include "FirstPersonCamera.h"
#include "PhysicsObject.h"
#include <math.h>

FirstPersonCamera::FirstPersonCamera() {
	m_firstPerson = NULL;
}

FirstPersonCamera::FirstPersonCamera(GameObject* firstPerson) {
	m_firstPerson = firstPerson;
}

void FirstPersonCamera::Update(float timestep) {

	if (m_firstPerson != NULL) {
		
		// We want to check if our first person kart is in a collision
		if (static_cast<Kart*>(m_firstPerson)->GetCollisionStatus()) {
			CameraShake();
			static_cast<Kart*>(m_firstPerson)->SetCollisionStatus(false);
		}

		float heading = m_firstPerson->GetYRotation();
		//Vector3 velocity = dynamic_cast<PhysicsObject*>(m_firstPerson)->GetVelocity();
		Matrix rotation = Matrix::CreateRotationY(heading);
		Vector3 rotatedOffset = Vector3::TransformNormal(
			m_offset,
			rotation);

		// Test for setting up camera shake
		if (time(NULL) - m_startShakeTime < 5) {
			Vector3 shakeOffset = Vector3::TransformNormal(Vector3((m_wobbleValue)*cos(m_shakeValue*timestep) * 0.5f, 0.0f, 10.0f), rotation);
			SetUp(shakeOffset);
			m_shakeValue += 10.0;
			m_wobbleValue -= (m_wobbleValue > 0.0) ? 0.02 : 0.0;
		}
		else {
			SetUp(Vector3(0.0f, 1.0f, 0.0f));
		}

		SetLookAt(m_firstPerson->GetPosition() + rotatedOffset);
		SetPosition(m_firstPerson->GetPosition() + Vector3(0.0f,4.0f,0.0f));
	}
	Camera::Update(timestep);
}

void FirstPersonCamera::SetFollowTarget(GameObject* target, Vector3 offset)
{
	m_firstPerson = target;
	m_offset = offset;
}

void FirstPersonCamera::CameraShake() {
	//OutputDebugString("CAMERASHAKE");
	m_startShakeTime = time(NULL);
	m_shakeValue = 0.0;
	m_wobbleValue = 1.0;
}
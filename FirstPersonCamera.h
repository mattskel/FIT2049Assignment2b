#ifndef FIRST_PERSON_CAMERA_H
#define FIRST_PERSON_CAMERA_H

#include "Camera.h"
#include "GameObject.h"
#include "Kart.h"
#include "time.h"

class FirstPersonCamera : public Camera {

private:
	GameObject* m_firstPerson;
	Vector3 m_offset;
	boolean m_shaking;
	int m_startShakeTime;
	double m_shakeValue;
	double m_wobbleValue;

public:
	FirstPersonCamera();
	FirstPersonCamera(GameObject* firstPerson);
	void Update(float timestep);
	void SetFollowTarget(GameObject* target, Vector3 offset);
	void CheckCollision();
	void CameraShake();

};
#endif
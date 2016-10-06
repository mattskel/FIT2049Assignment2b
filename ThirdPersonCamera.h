/*
* The third person camera follows the player
* The object implements lerping to have a delayed chase
* As the player speeds away the camera falls behind
* as the player slows down the camera catches up
*/

#ifndef THIRD_PERSON_CAMERA_H
#define THIRD_PERSON_CAMERA_H

#include "Camera.h"
#include "GameObject.h"
#include "PhysicsObject.h"

class ThirdPersonCamera : public Camera {
private:
	GameObject* m_objectToFollow;
	Vector3 m_offset;

public:
	ThirdPersonCamera();
	ThirdPersonCamera(GameObject* target, Vector3 offset);

	void Update(float timestep);

	void SetFollowTarget(GameObject* target, Vector3 offset);

};

#endif
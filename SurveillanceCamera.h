/*	FIT2049 - Week 4 Solution
*	SurveillanceCamera.h
*	Created by Mike Yeates - 2016 - Monash University
*	A surveillance camera always looks at something but never moves
*	It's like a security camera mounted on a wall
*/

#ifndef SURVEILLANCE_CAMERA_H
#define SURVEILLANCE_CAMERA_H

#include "Camera.h"
#include "GameObject.h"

class SurveillanceCamera : public Camera
{
private:
	// The object this camera will focus on
	GameObject* m_lookAtTarget;

	// A dampen of 0 means we ignore our target
	// A dampen of 1 means we are looking directly at it
	float m_dampen;

public:
	SurveillanceCamera();
	SurveillanceCamera(GameObject* target, float dampen);

	void Update(float timestep);

	void SetTarget(GameObject* target, float dampen);
};

#endif
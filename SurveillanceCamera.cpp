/*	FIT2049 - Week 4 Solution
*	SurveillanceCamera.cpp
*	Created by Mike Yeates - 2016 - Monash University
*	Implementation of SurveillanceCamera.cpp
*/

#include "SurveillanceCamera.h"

SurveillanceCamera::SurveillanceCamera()
{
	SetTarget(NULL, 0.0f);
}

SurveillanceCamera::SurveillanceCamera(GameObject* target, float dampen)
{
	SetTarget(target, dampen);
}

void SurveillanceCamera::Update(float timestep)
{
	// Use accessors so parent's "dirty" booleans get flipped.
	// Not making anything protected in Camera has forced
	// us to use these accessors which is a good thing.
	SetLookAt(m_lookAtTarget->GetPosition() * m_dampen);

	// Give our parent a chance to rebuild the view and projection
	// matrices if any important data has changed
	Camera::Update(timestep);

}

void SurveillanceCamera::SetTarget(GameObject* target, float dampen)
{
	m_lookAtTarget = target;
	m_dampen = dampen;
}
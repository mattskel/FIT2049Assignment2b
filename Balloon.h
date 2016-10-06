/*
 * Balloon Class
 * Balloon follows each Kart
 * When a Kart is hit the Balloon is lost and floats into the air
 */

#ifndef BALLOON_H
#define BALLOON_H

#include "GameObject.h"

class Kart;

class Balloon : public GameObject {
private:
	Vector3 m_displacement;
	Vector3 m_currentPosition;
	Kart* m_parentKart;
	boolean m_lifeLost;
	int m_timeStart;

public:
	Balloon(Mesh* mesh,
		Shader* shader,
		Texture* texture,
		Material* mat,
		Vector3 displacement,
		Kart* parentKart);

	void Update(float timestep);
	void SetLifeLost() { m_lifeLost = true; }
};

#endif
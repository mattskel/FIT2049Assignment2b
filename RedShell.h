/*
 * Red shell objects can follow a kart
 * When red shell objects are created they are given a pointer to a kart
 * the red shell updates its position to follow the position of the kart
 */

#ifndef RED_SHELL_H
#define RED_SHELL_H

#include "Shell.h"

#include <vector>

class RedShell : public Shell {
private:
	std::vector<Kart*>* m_karts;	// Pointer to list of active Kart pointers
	Kart* m_targetKart;	// The kart to follow
	Vector3 m_targetPosition;
public: 
	RedShell(Mesh* mesh,
	Shader* shader,
	Texture* texture,
	Material* mat, 
	Vector3 position,
	Vector3 direction,
	float rotY,
	std::vector<Kart*>* karts);

	void Update(float timestep);

	Vector3 GetLocalForward();
};

#endif
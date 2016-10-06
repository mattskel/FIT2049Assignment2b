/*
 * EnemyKart Class
 * EnemyKart either goes to a random position or follows the player
 * When the enemy kart detects an item box nearby, the target position is changed to the ItemBox
 *
 */

#ifndef ENEMY_KART_H
#define ENEMY_KART_H

#include "PhysicsObject.h"
#include "Kart.h"
#include "InputController.h"


class EnemyKart : public Kart
{
private:

	const int WORLD_WIDTH = 299;
	const int WORLD_DEPTH = 299;

	Vector3 m_targetPosition;
	Vector3 GetRandomPosition();

	boolean m_targetIsItemBox;	// Checks whether the current target is an item box
	boolean m_chasingPlayer;	// Determine if we are chasing the player or not
	Kart* m_playerKart;	// If EnemyKart is chasing the player uses this to determine position
	int m_startNewTarget;	// Could get stuck in a loop, want to look for a new target after some time

public:
	EnemyKart(Mesh* mesh,
		Shader* shader,
		Texture* texture,
		Material* mat,
		Vector3 position,
		int flag);

	void SetPlayerKart(Kart* playerKart) { m_playerKart = playerKart; }
	void SetChasingPlayer() { m_chasingPlayer = true; }

	void Update(float timestep);
};

#endif
/*
 * Shell object is used to represent the sell items in the game.
 * A shell can be either a greenshell or a red shell
 * the type of shell determines how it will move in the game
 * When a shell collides with a wall it bounces off
 * When a shell collides with a player it is destroyed
 */

#ifndef SHELL_H
#define SHELL_H

#include "PhysicsObject.h"
#include "Kart.h"
#include "Wall.h"

#include <vector>

class Shell : public PhysicsObject {
protected:
	CBoundingBox m_boundingBox;
	Vector3 m_direction;
	float m_moveSpeed;
	float m_turnSpeed;
public:
	Shell(Mesh* mesh,
		Shader* shader,
		Texture* texture,
		Material* mat,
		Vector3 position,
		Vector3 direction);

	CBoundingBox GetBounds() { return m_boundingBox; }

	Vector3 GetLocalForward();

	// Collisions
	void OnObjectCollisionEnter();
	void OnKartCollisionEnter();
	void OnWallCollisionEnter(Wall* other);

};

#endif
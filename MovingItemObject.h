#ifndef MOVING_ITEM_OBJECT_H
#define MOVING_ITEM_OBJECT_H

#include "PhysicsObject.h"
#include "Kart.h"
#include "Wall.h"

class MovingItemObject : public PhysicsObject {

private:

	float m_moveSpeed;
	CBoundingBox m_boundingBox;

	Vector3 currentDirection;

public:

	MovingItemObject(Mesh* mesh,
		Shader* shader,
		Texture* texture,
		Material* mat,
		Vector3 position,
		Vector3 direction);

	void Update(float timestep);

	CBoundingBox GetBounds() { return m_boundingBox; }

	Vector3 GetLocalForward();

	void OnObjectCollisionEnter();
	//void OnObjectCollisionStay();
	//void OnObjectCollisionExit();

	void OnKartCollisionEnter();

	void OnWallCollisionEnter(Wall* other);
};

#endif
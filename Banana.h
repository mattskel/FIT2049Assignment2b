/*
 * Banana Class
 * Banana is an item dropped by one of the Karts
 */

#ifndef BANANA_H
#define BANANA_H

#include "StaticObject.h"

class Banana : public StaticObject {
private:
	CBoundingBox m_boundingBox;
public:
	Banana(Mesh* mesh,
		Shader* shader,
		Texture* texture,
		Material* mat,
		Vector3 position);

	CBoundingBox GetBounds() { return m_boundingBox; }

	void OnKartCollisionEnter();
};


#endif
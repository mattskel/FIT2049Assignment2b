#ifndef ITEM_BOX_H
#define ITEM_BOX_H

#include "GameObject.h"
#include "Kart.h"
#include "SpinningObject.h"

#include <time.h>

// Forward declare Kart to prevent loops?
class Kart;

class ItemBox : public SpinningObject
{
private:
	CBoundingBox m_boundingBox;
	int itemValue;
	boolean recentCollision;
	float m_floatCycleRoot;
	float m_currentFloatValue;
	float m_tmpUniformScale;
	boolean m_badBox;
	int m_timeStart;

public:
	ItemBox(Mesh* mesh, Shader* shader, Texture* texture, Material* mat, Vector3 position);

	void Update(float timestep);

	void SetBadBox() { m_badBox = true; }

	CBoundingBox GetBounds() { return m_boundingBox; }
	void OnKartCollisionEnter(Kart* other);
	void OnKartCollisionStay(Kart* other);
	void OnKartCollisionExit(Kart* other);
};

#endif
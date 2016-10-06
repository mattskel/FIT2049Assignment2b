/*
* The Wall class is used for collisions
* The Wall class stores a bounding box to represent one of the four walls
* The wall also has a normal vector to the face of the wall
* The normal vector is used to calculate the angle objects should rebound off
*/

#ifndef WALL_H
#define WALL_H

#include "StaticObject.h"

class Wall : public GameObject {

private:
	CBoundingBox m_boundingBox;
	Vector3 m_localFace;

public:

	Wall();
	void Update(float timestep);
	void SetBounds(CBoundingBox);
	void SetFace(Vector3 localFace);
	CBoundingBox GetBounds() { return m_boundingBox; }
	Vector3 GetLocalFace() { return m_localFace; }

};

#endif
#include "Wall.h"

Wall::Wall() : GameObject() {
}

void Wall::Update(float timestep) {};

void Wall::SetBounds(CBoundingBox boundingBox) {
	m_boundingBox = boundingBox;
}

void Wall::SetFace(Vector3 localFace) {
	m_localFace = localFace;
}
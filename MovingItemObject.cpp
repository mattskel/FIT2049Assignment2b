#include "MovingItemObject.h"

MovingItemObject::MovingItemObject(Mesh* mesh,
	Shader* shader,
	Texture* texture,
	Material* mat,
	Vector3 position,
	Vector3 direction) :
	PhysicsObject(mesh, shader, texture, mat, position + 20*direction) {

	m_moveSpeed = 7.0f; // Alter these to change the speed
	m_frictionAmount = 3.0f;

	m_boundingBox = CBoundingBox(GetPosition() + m_mesh->GetMin(),
		GetPosition() + m_mesh->GetMax());

	currentDirection = direction;
}

void MovingItemObject::Update(float timestep) {

	ApplyForce(currentDirection * m_moveSpeed * timestep);

	m_boundingBox.SetMin(GetPosition() + m_mesh->GetMin());
	m_boundingBox.SetMax(GetPosition() + m_mesh->GetMax());

	PhysicsObject::Update(timestep);
}

Vector3 MovingItemObject::GetLocalForward() {

	Vector3 worldForward = Vector3(0, 0, 1);
	Matrix heading = Matrix::CreateRotationY(m_rotY);
	Vector3 localForward = Vector3::TransformNormal(worldForward, heading);

	return localForward;
}

void MovingItemObject::OnObjectCollisionEnter() {

}

void MovingItemObject::OnKartCollisionEnter() {
	m_status = 0;
}

void MovingItemObject::OnWallCollisionEnter(Wall* other) {

}
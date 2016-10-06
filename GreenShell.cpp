#include "GreenShell.h"

GreenShell::GreenShell(Mesh* mesh,
	Shader* shader,
	Texture* texture,
	Material* mat,
	Vector3 position,
	Vector3 direction) :
	Shell(mesh, shader, texture, mat, position, direction) {}

void GreenShell::Update(float timestep) {
	ApplyForce(m_direction * m_moveSpeed * timestep);

	m_boundingBox.SetMin(GetPosition() + m_mesh->GetMin());
	m_boundingBox.SetMax(GetPosition() + m_mesh->GetMax());

	PhysicsObject::Update(timestep);
}
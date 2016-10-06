#include "Shell.h"

Shell::Shell(Mesh* mesh,
	Shader* shader,
	Texture* texture,
	Material* mat,
	Vector3 position,
	Vector3 direction) :
	PhysicsObject(mesh, shader, texture, mat, position + 20 * direction) {

	m_moveSpeed = 5.0f; // Alter these to change the speed
	m_turnSpeed = 4.0f;
	m_frictionAmount = 3.0f;
	m_boundingBox = CBoundingBox(GetPosition() + m_mesh->GetMin(),
		GetPosition() + m_mesh->GetMax());
	m_direction = direction;
}

Vector3 Shell::GetLocalForward() {

	Vector3 worldForward = Vector3(0, 0, 1);
	Matrix heading = Matrix::CreateRotationY(m_rotY);
	Vector3 localForward = Vector3::TransformNormal(worldForward, heading);
	return localForward;
}

void Shell::OnObjectCollisionEnter() {}
void Shell::OnKartCollisionEnter() { m_status = 0; }

void Shell::OnWallCollisionEnter(Wall* other) {
	
	Vector3 velocity = GetVelocity();
	Vector3 wallFace = other->GetLocalFace();
	float dotProd = m_direction.x * wallFace.x +
					m_direction.z * wallFace.z;

	m_direction = m_direction - wallFace * 2 * dotProd;
	ApplyForce(4.0 * (m_direction));
}
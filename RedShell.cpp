#include "RedShell.h"

RedShell::RedShell(Mesh* mesh,
	Shader* shader,
	Texture* texture,
	Material* mat,
	Vector3 position,
	Vector3 direction,
	float rotY,
	std::vector<Kart*>* karts) : 
	Shell(mesh, shader, texture, mat, position, direction) {

	m_rotY = rotY;

	m_karts = karts;
	// Initialise the target Kart
	float maxDotProduct = -1;
	for (Kart* kart : *m_karts) {
		Vector3 kartPosition = kart->GetPosition();
		Vector3 kartVector = kartPosition - m_position;
		
		// Get the vector length
		float vectorLength = kartVector.Length();
		Vector3 normalKartVector = Vector3(kartVector.x / vectorLength,
											kartVector.y / vectorLength,
											kartVector.z / vectorLength);
		
		// Get the dot product to determine tha size of the angle
		float dotProduct = normalKartVector.x * m_direction.x +
							normalKartVector.y * m_direction.y +
							normalKartVector.z * m_direction.z;
		if (dotProduct > maxDotProduct) {
			maxDotProduct = dotProduct;
			m_targetKart = kart;
		}
	}
}
// Updates the position to follow the target kart
void RedShell::Update(float timestep) {

	Vector3 targetPosition = m_targetKart->GetPosition();
	Vector3 localForward = GetLocalForward();
	Vector3 targetVector = targetPosition - GetPosition();

	float targetVectorLength = targetVector.Length();
	Vector3 normalTargetVector = Vector3(targetVector.x / targetVectorLength,
										targetVector.y / targetVectorLength,
										targetVector.z / targetVectorLength);
	
	// Use the cross product to determine what direction to turn
	float crossProduct = localForward.x * normalTargetVector.z - 
						localForward.z*normalTargetVector.x;

	// Turn RedShell
	if (crossProduct > 0.01f) {
		m_rotY -= m_turnSpeed * timestep;
	}
	else if (crossProduct < -0.01f) {
		m_rotY += m_turnSpeed * timestep;
	}

	// Apply the force in the forward direction
	ApplyForce(localForward * m_moveSpeed * timestep);

	// Move collider
	m_boundingBox.SetMin(GetPosition() + m_mesh->GetMin());
	m_boundingBox.SetMax(GetPosition() + m_mesh->GetMax());

	PhysicsObject::Update(timestep);
}

Vector3 RedShell::GetLocalForward() {

	Vector3 worldForward = Vector3(0, 0, 1);
	Matrix heading = Matrix::CreateRotationY(m_rotY);
	Vector3 localForward = Vector3::TransformNormal(worldForward, heading);

	return localForward;
}
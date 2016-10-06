#include "SpinningObject.h"

SpinningObject::SpinningObject() : GameObject()
{
	m_spinningSpeed = 0.0f;
}

SpinningObject::SpinningObject(Mesh* mesh, Shader* shader, float spinSpeed) : 
	GameObject(mesh, shader)
{
	m_spinningSpeed = spinSpeed;
}

SpinningObject::SpinningObject(Mesh* mesh, Shader* shader, Vector3 position, Material* mat, float spinSpeed) : 
	GameObject(mesh, shader, mat, position)
{
	m_spinningSpeed = spinSpeed;
}

SpinningObject::SpinningObject(Mesh* mesh, Shader* shader, Texture* texture, Material* mat, float spinSpeed) :
	GameObject(mesh, shader, texture, mat)
{
	m_spinningSpeed = spinSpeed;
}

SpinningObject::SpinningObject(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position, Material* mat, float spinSpeed) :
	GameObject(mesh, shader, texture, mat, position)
{
	m_spinningSpeed = spinSpeed;
}

SpinningObject::~SpinningObject() { }

void SpinningObject::Update(float timestep)
{
	//m_rotY += m_spinningSpeed * timestep;
}
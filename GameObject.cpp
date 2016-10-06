#include "GameObject.h"
#include "MathsHelper.h"

GameObject::GameObject()
{
	m_position = Vector3::Zero;
	m_rotX = m_rotY = m_rotZ = 0;
	SetUniformScale(1.0f);
	m_mesh = NULL;
	m_texture = NULL;
	m_shader = NULL;
	m_status = 1;
}
GameObject::GameObject(Mesh* mesh, Shader* shader)
{
	m_position = Vector3::Zero;
	m_rotX = m_rotY = m_rotZ = 0;
	SetUniformScale(1.0f);
	m_mesh = mesh;
	m_shader = shader;
	m_texture = NULL;
	m_material = NULL;
	m_status = 1;
}
GameObject::GameObject(Mesh* mesh, Shader* shader, Material* mat, Vector3 position)
{
	m_position = position;
	m_rotX = m_rotY = m_rotZ = 0;
	SetUniformScale(1.0f);
	m_mesh = mesh;
	m_shader = shader;
	m_texture = NULL;
	m_material = mat;
	m_status = 1;
}
GameObject::GameObject(Mesh* mesh, Shader* shader, Texture* texture, Material* mat)
{
	m_position = Vector3::Zero;
	m_rotX = m_rotY = m_rotZ = 0;
	SetUniformScale(1.0f);
	m_mesh = mesh;
	m_shader = shader;
	m_texture = texture;
	m_material = mat;
	m_status = 1;
}
GameObject::GameObject(Mesh* mesh, Shader* shader, Texture* texture, Material* mat, Vector3 position)
{
	m_position = position;
	m_rotX = m_rotY = m_rotZ = 0;
	SetUniformScale(1.0f);
	m_mesh = mesh;
	m_shader = shader;
	m_texture = texture;
	m_material = mat;
	m_status = 1;
}

GameObject::~GameObject() {}

void GameObject::Render(Direct3D* direct3D, Camera* currentCamera)
{
	if (m_mesh)
	{
		// Our engine now works in degrees, and converts to radians below
		Matrix world = Matrix::CreateScale(m_scaleX, m_scaleY, m_scaleZ) * Matrix::CreateFromYawPitchRoll(m_rotY, m_rotX, m_rotZ) * Matrix::CreateTranslation(m_position);
		m_mesh->Render(direct3D, world, currentCamera, m_shader, m_texture, m_material);
	}

}
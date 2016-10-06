#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Direct3D.h"
#include "Mesh.h"
#include "Collisions.h"

class GameObject
{
protected:
	Vector3 m_position;
	float m_rotX, m_rotY, m_rotZ;
	float m_scaleX, m_scaleY, m_scaleZ;
	int m_status;	// GameObject status

	Mesh* m_mesh;
	Texture* m_texture;
	Shader* m_shader;
	Material* m_material;

public:	
	GameObject();
	GameObject(Mesh* mesh, Shader* shader);
	GameObject(Mesh* mesh, Shader* shader, Material* mat);
	GameObject(Mesh* mesh, Shader* shader, Material* mat, Vector3 position);
	GameObject(Mesh* mesh, Shader* shader, Texture* texture, Material* mat);
	GameObject(Mesh* mesh, Shader* shader, Texture* texture, Material* mat, Vector3 position);
	virtual ~GameObject();

	// Accessors
	Vector3 GetPosition() { return m_position; }
	float GetXRotation() { return m_rotX; }
	float GetYRotation() { return m_rotY; }
	float GetZRotation() { return m_rotZ; }
	float GetXScale() { return m_scaleX; }
	float GetYScale() { return m_scaleY; }
	float GetZScale() { return m_scaleZ; }
	Mesh* GetMesh() { return m_mesh; }
	Texture* GetTexture() { return m_texture; }
	Shader* GetShader() { return m_shader; }

	int GetStatus() { return m_status; }	// Returns the objects status

	// Mutators
	void SetPosition(Vector3 pos) { m_position = pos; }
	void SetXRotation(float xRot) { m_rotX = xRot; }
	void SetYRotation(float yRot) { m_rotY = yRot; }
	void SetZRotation(float zRot) { m_rotZ = zRot; }
	void SetXScale(float xScale) { m_scaleX = xScale; }
	void SetYScale(float yScale) { m_scaleY = yScale; }
	void SetZScale(float zScale) { m_scaleZ = zScale; }
	void SetUniformScale(float scale) { m_scaleX = m_scaleY = m_scaleZ = scale; }
	void SetMesh(Mesh* mesh) { m_mesh = mesh; }
	void SetTexture(Texture* texture) { m_texture = texture; }
	void SetShader(Shader* shader) { m_shader = shader; }

	// GameObject is abstract as Update is pure virtual
	// You could make a strong case that GameObject shouldn't be abstract,
	// however we're really just doing it as an excuse to write an abstract class.
	virtual void Update(float timestep) = 0;
	virtual void Render(Direct3D* renderer, Camera* currentCamera);
};

#endif
/*	FIT2049 - Example Code
*	Mesh.h
*	Created by Elliott Wilson & Mike Yeates - 2016 - Monash University
*	This class represents an indexed Mesh of 3D Vertices.
*	It is capable of loading triangulated OBJ files as well as creating simple primitives
*	It also manages a collection of loaded meshes so we can fetch them statically from any object
*/

#ifndef MESH_H
#define MESH_H

#include "Direct3D.h"
#include "DirectXTK/SimpleMath.h"
#include "Camera.h"
#include "Texture.h"
#include "SceneLighting.h"

class Mesh
{
private:
	struct Vertex			//This struct represents a single vertex of our mesh
	{
		Vector3 position;	//It is made up of a position
		Color colour;		//A colour
		Vector3 normal;		//A normal, which defines which way the face is "facing", these are important for lighting calculations
		Vector2 texCoord;	//and a texture coordinate, these are used to determine which part of a texture belongs to this vertex
	};

	Mesh::Mesh(Direct3D* direct3D, const char* identifier, int numVertices, int numIndices, Vertex* vertexData, unsigned long* indexData, D3D11_PRIMITIVE_TOPOLOGY topology);

	// Not using filename to identify a mesh so we can easily rename and not have to change lots in our code
	// Note these must be unique as we use them as keys in our map
	const char* m_identifier;

	int m_vertexCount;				//We store a count of all of the vertices
	int m_indexCount;				//As well as a count of all of the indices
	ID3D11Buffer* m_vertexBuffer;	//We use Direct3D Buffers to store the verts...
	ID3D11Buffer* m_indexBuffer;	//... and the indices in graphics memory

	Vector3 m_minVector;	//For our bounding boxes we need to know the min position of the mesh...
	Vector3 m_maxVector;	//... and the max position
	Vector3 m_centre;		//For the bounding spheres we need to know the centre point...
	float m_radius;			//... and the overall radius.

	D3D11_PRIMITIVE_TOPOLOGY m_topology; // How will Direct3D interpret the indices for this mesh (basically list or strip)

	// Static manager helpers
	// We keep track of every mesh created so we can fetch meshes anywhere in our game
	typedef std::map<std::string, Mesh*> MeshMap;
	static MeshMap meshMap;

	bool InitialiseBuffers(Direct3D* direct3D, Vertex* vertexData, unsigned long* indexData);	//This method takes vertex and index data and creates the Direct3D buffers
	
public:
	~Mesh();

	void Render(Direct3D* direct3D, Matrix world, Camera* cam, Shader* shader, Texture* texture, Material* material);		//Renders the mesh data according to the mesh shader and the world matrix supplied

	// Accessors
	int GetVertexCount() { return m_vertexCount; }
	int GetIndexCount() { return m_indexCount; }
	const char* GetIdentifier() { return m_identifier; }
	Vector3 GetMin() { return m_minVector; }
	Vector3 GetMax() { return m_maxVector; }
	Vector3 GetCentre() { return m_centre; }
	float GetRadius() { return m_radius; }

	// Mutators
	void SetBounds(Vector3 min, Vector3 max);
	void SetBounds(Vector3 centre, float radius);

	// Static manager helpers
	static bool LoadFromFile(LPCWSTR filename, const char* identifier, Direct3D* direct3D);

	static Mesh* GetMesh(const char* identifier);
	static void ReleaseMesh(Mesh* mesh);
	static void ReleaseAllMeshes();
	
};

#endif
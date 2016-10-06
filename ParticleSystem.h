#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <d3dcompiler.h>
#include "Direct3D.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectXTK/SimpleMath.h"
#include <vector>

using namespace DirectX::SimpleMath;

#define MAX_BUFFER_SIZE 4096

class ParticleSystem
{
private:
	struct Particle
	{
		Vector3 position;
		Vector3 velocity;
		Color colour;
		float radius;

		// We use these to determine this particles age
		float timeCreated;
		float lifetime;

		// Using a linked list to store particles, so each particle knows which one comes next.
		// This allows very quick insertion without shuffling everything along.
		Particle* next;
	};
	
	// Particles don't need many vertex attributes - just a position
	struct ParticleVertex
	{
		Vector3 position;	
		Color colour;
		float radius;
	};
	struct ParticleConstantsBuffer
	{
		Matrix worldViewProjection;
		//----------------------------------- (16 byte boundary)
		Vector3 cameraPosition;
		float padding;
		//----------------------------------- (16 byte boundary)
	};
	
	static ID3D11VertexShader* s_vertexShader;
	static ID3D11PixelShader* s_pixelShader;
	static ID3D11GeometryShader* s_geometryShader;
	static ID3D11InputLayout* s_inputLayout;
	static ID3D11Buffer* s_constantsBuffer;
	static ID3D11SamplerState* s_textureSampler;
	
	ID3D11Buffer* m_vertexBuffer;

	// Finding a "free" particle is now very fast as we maintain two collections
	Particle* m_activeParticleList; // These are currently alive and being simulated
	Particle* m_freeParticleList; // These are in a disabled state waiting to be re-used

	int m_activeCount; // How many particles are we currently simulating
	float m_currentTime; // Total time elapsed
	float m_lastUpdate; // Time elapsed when we released our last particle

	float m_minParticleRadius;
	float m_maxParticleRadius;
	Texture* m_particleTexture;
	std::vector<Color> m_particleColours;

	int m_maxParticles;
	int m_numberToRelease;
	float m_releaseInterval;
	float m_particleLifetimeMin;
	float m_particleLifetimeMax;

	// Only one of these will be used. Box will take priority over sphere.
	Vector3 m_emitZoneExtents; // Create new particles inside a box
	float m_emitZoneRadius; // Create new particles inside a sphere

	Vector3 m_systemPosition;
	Vector3 m_systemVelocity;
	Vector3 m_systemGravity;
	Vector3 m_systemWind;
	bool m_useWind;
	float m_velocityVariance;

	static bool InitialiseShaders(Direct3D* renderer, LPCWSTR shaderFilename);
	static bool InitialiseBuffers(Direct3D* renderer);
	static bool InitialiseSamplers(Direct3D* renderer);

	Vector3 GetStartPosition();

public:
	ParticleSystem();
	~ParticleSystem();

	void SetTexture(Texture* texture) { m_particleTexture = texture; }

	void SetMaxParticles(int max) { m_maxParticles = max; }
	void SetNumberToRelease(int release) { m_numberToRelease = release; }
	void SetReleaseInterval(float interval) { m_releaseInterval = interval; }
	void SetLifetime(float min, float max) { m_particleLifetimeMin = min; m_particleLifetimeMax = max; }
	void SetRadius(float min, float max) { m_minParticleRadius = min; m_maxParticleRadius = max; }
	void AddColour(Color colour) { m_particleColours.push_back(colour); }
	void SetPosition(Vector3 position) { m_systemPosition = position; }
	void SetEmitArea(Vector3 extents) { m_emitZoneExtents = extents; }
	void SetEmitArea(float radius) { m_emitZoneRadius = radius; }
	void SetVelocity(Vector3 velocity) { m_systemVelocity = velocity; }
	void SetGravity(Vector3 gravity) { m_systemGravity = gravity; }
	void SetWind(Vector3 wind) { m_systemWind = wind; }
	void UseWind(bool wind) { m_useWind = wind; }
	void SetVelocityVariance(float variance) { m_velocityVariance = variance; }

	bool Initialise(Direct3D* renderer);
	void Update(float timestep);
	void Render(Direct3D* renderer, Camera* cam);
	void Release();

	static bool InitialiseSystem(Direct3D* renderer, LPCWSTR shaderFilename);
	static void Begin(Direct3D* renderer);
	static void End(Direct3D* renderer);
	static void ShutdownSystem();
};

#endif
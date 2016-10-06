#include "ParticleSystem.h"
#include "MathsHelper.h"

ParticleSystem::ParticleSystem()
{
	m_particleTexture = NULL;
	
	m_vertexBuffer = NULL;

	m_activeParticleList = NULL;
	m_freeParticleList = NULL;

	m_activeCount = 0;
	m_currentTime = 0;
	m_lastUpdate = 0;

	m_minParticleRadius = 1.0f;
	m_maxParticleRadius = 1.0f;
	
	m_maxParticles = 0;
	m_numberToRelease = 0;
	m_releaseInterval = 0;
	m_particleLifetimeMin = 0;
	m_particleLifetimeMax = 0;

	m_emitZoneExtents = Vector3::Zero;
	m_emitZoneRadius = 0.0f;

	m_systemPosition = Vector3::Zero;
	m_systemVelocity = Vector3::Zero;
	m_systemGravity = Vector3::Zero;
	m_systemWind = Vector3::Zero;
	m_useWind = false;
	m_velocityVariance = 0;


}
ParticleSystem::~ParticleSystem()
{

}

bool ParticleSystem::Initialise(Direct3D* renderer)
{
	D3D11_BUFFER_DESC vertexBufferDescription;

	vertexBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDescription.ByteWidth = sizeof(ParticleVertex) * MAX_BUFFER_SIZE;
	vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDescription.MiscFlags = 0;
	vertexBufferDescription.StructureByteStride = 0;

	if (FAILED(renderer->GetDevice()->CreateBuffer(&vertexBufferDescription, NULL, &m_vertexBuffer)))
	{
		return false;
	}

	return true;
}

Vector3 ParticleSystem::GetStartPosition()
{
	// Called whenever we want to spawn a new particle
	// Checks whether an emit zone was specified (box takes priority)

	if (m_emitZoneExtents != Vector3::Zero)
	{
		// Return a position somewhere inside the emit box
		return MathsHelper::GetPointWithinBoxCenterExtents(m_systemPosition, m_emitZoneExtents);
	}
	if (m_emitZoneRadius > 0.0f)
	{
		// Return a position somewhere inside the emit sphere
		return MathsHelper::GetPointWithinSphere(m_systemPosition, m_emitZoneRadius);
	}

	// No emit zone was specified. Emit from the system position.
	return m_systemPosition;

}

void ParticleSystem::Update(float timestep)
{
	Particle* currentParticle = NULL;
	Particle** particleListPointer;

	// Update our particle system timer
	m_currentTime += timestep;

	// Start at the head of the active list
	particleListPointer = &m_activeParticleList;

	// Loop through all the active particles
	// We're using a linked list of particles so this loop might look a bit weird
	while (*particleListPointer)
	{
		// Set a pointer to the head
		currentParticle = *particleListPointer;
		
		// Determine the age of this particle
		float timePassed = m_currentTime - currentParticle->timeCreated;

		// Is it time to kill this particle
		if (timePassed >= currentParticle->lifetime)
		{
			// Time is up, put the particle back on the free list...
			*particleListPointer = currentParticle->next;
			currentParticle->next = m_freeParticleList;
			m_freeParticleList = currentParticle;

			// We're now simulating one less particle
			m_activeCount--;
		}
		else
		{
			// This particle is still alive - update particle position and velocity
			// This is basically the force accumulation stuff we did a few week ago

			// Update velocity with respect to Gravity (Constant Acceleration)
			currentParticle->velocity += m_systemGravity * timestep;

			// Update velocity with respect to Wind (Acceleration based on  difference of vectors)
			if (m_useWind)
				currentParticle->velocity += (m_systemWind - currentParticle->velocity) * timestep;
			
			// Finally, update position with respect to velocity
			currentParticle->position += currentParticle->velocity * timestep;

			// Move to the next particle in the list for the next iteration
			particleListPointer = &currentParticle->next;
		}
	}

	//-------------------------------------------------------------------------
	// Emit new particles in accordance to the flow rate...
	// 
	// NOTE: The system operates with a finite number of particles.
	//       New particles will be created until the max amount has
	//       been reached, after that, only old particles that have
	//       been recycled can be reintialized and used again.
	//-------------------------------------------------------------------------

	// Is it time to release a new burst of particles?
	if (m_currentTime - m_lastUpdate > m_releaseInterval)
	{
		// Reset update timing...
		m_lastUpdate = m_currentTime;

		// Emit new particles at specified flow rate...
		for (unsigned int i = 0; i < m_numberToRelease; i++)
		{
			// Do we have any free particles to put back to work?
			if (m_freeParticleList)
			{
				// If so, hand over the first n free to be reused
				currentParticle = m_freeParticleList;

				// Then make the next free particle in the list next to go!
				m_freeParticleList = currentParticle->next;

				// We're about to move the current particle out of the free list and into the active list
				// Lose any links with the current list by setting next to NULL
				currentParticle->next = NULL;
			}
			else
			{
				// There are no free particles to recycle...
				// We'll have to create a new one from scratch!
				if (m_activeCount < m_maxParticles)
				{
					currentParticle = new Particle();
					if (!currentParticle)
						return;

					currentParticle->next = NULL;
				}
			}

			// We now have a new particle, whether it was just instantiated or recycled doesn't matter

			if (m_activeCount < m_maxParticles)
			{
				// Make it the new head of the active list
				// This essentially adds it to the active list

				// Our next particle is the old head of the active list
				currentParticle->next = m_activeParticleList;

				// And we are now the new head of the active list
				m_activeParticleList = currentParticle;

				// Set the attributes for our new particle...
				currentParticle->position = GetStartPosition();
				currentParticle->radius = MathsHelper::RandomRange(m_minParticleRadius, m_maxParticleRadius);
				currentParticle->colour = m_particleColours[MathsHelper::RandomRange(0, m_particleColours.size() - 1)];
				currentParticle->lifetime = MathsHelper::RandomRange(m_particleLifetimeMin, m_particleLifetimeMax);
				currentParticle->timeCreated = m_currentTime;
				currentParticle->velocity = m_systemVelocity;

				if (m_velocityVariance != 0.0f)
				{
					currentParticle->velocity += MathsHelper::RandomUnitVector() * m_velocityVariance;
				}

				m_activeCount++;
			}
		}
	}
}

void ParticleSystem::Render(Direct3D* renderer, Camera* cam)
{
	// This is stuff our mesh class usually does, however a particle isn't really a mesh so it does it itself

	// We want to draw everything in the active list, so store a pointer to the head
	Particle* currentParticle = m_activeParticleList;
	ParticleVertex* verts;
	ParticleConstantsBuffer* constants;
	D3D11_MAPPED_SUBRESOURCE vertexResource;
	D3D11_MAPPED_SUBRESOURCE constantsResource;

	// Our first step of rendering is to map a chunk of memory from the GPU into system memory so we can fill out the vertex buffer
	if (FAILED(renderer->GetDeviceContext()->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &vertexResource)))
	{
		return;
	}
	ParticleVertex* start = (ParticleVertex*)vertexResource.pData;
	verts = (ParticleVertex*)vertexResource.pData;

	// Loop through the active list and add its position to the vertex buffer
	while (currentParticle)
	{
		verts->position = currentParticle->position;
		verts->colour = currentParticle->colour;
		verts->radius = currentParticle->radius;
		verts++;

		// Move to the next particle on the next iteration
		currentParticle = currentParticle->next;
	}
	
	// Copy the vertex buffer back to GPU memory
	renderer->GetDeviceContext()->Unmap(m_vertexBuffer, 0);

	// Set Constant buffer
	if (FAILED(renderer->GetDeviceContext()->Map(s_constantsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &constantsResource)))
	{
		return;
	}

	constants = (ParticleConstantsBuffer*)constantsResource.pData;

	Matrix wvp = cam->GetView() * cam->GetProjection();

	constants->worldViewProjection = wvp.Transpose();
	constants->cameraPosition = cam->GetPosition();
	constants->padding = 0.0f;

	renderer->GetDeviceContext()->Unmap(s_constantsBuffer, 0);

	// The GPU now has a list of vertices we want to draw and our constant buffer has been filled out
	
	// Usually we draw triangle lists - this time we're drawing a point list as we have no faces
	// Because we're not forming faces, we don't need indices or an index buffer
	renderer->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	// The GPU will read our vertex buffer in ParticleVertex size steps
	unsigned int stride = sizeof(ParticleVertex);
	unsigned int offset = 0;
	renderer->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	
	// The last piece of data our shader needs is the texture we want to use
	ID3D11ShaderResourceView* textureView = m_particleTexture->GetShaderResourceView();
	renderer->GetDeviceContext()->PSSetShaderResources(0, 1, &textureView);
	
	// Instruct the GPU to draw our vertex buffer
	renderer->GetDeviceContext()->Draw(m_activeCount, 0);
}
void ParticleSystem::Release()
{
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = NULL;
	}

	while (m_activeParticleList)
	{
		Particle* p = m_activeParticleList;
		m_activeParticleList = p->next;
		delete p;
	}

	while (m_freeParticleList)
	{
		Particle* p = m_freeParticleList;
		m_freeParticleList = p->next;
		delete p;
	}
}

//Static Methods

ID3D11VertexShader* ParticleSystem::s_vertexShader = NULL;
ID3D11PixelShader* ParticleSystem::s_pixelShader = NULL;
ID3D11GeometryShader* ParticleSystem::s_geometryShader = NULL;
ID3D11InputLayout* ParticleSystem::s_inputLayout = NULL;
ID3D11Buffer* ParticleSystem::s_constantsBuffer = NULL;
ID3D11SamplerState* ParticleSystem::s_textureSampler = NULL;

bool ParticleSystem::InitialiseSystem(Direct3D* renderer, LPCWSTR shaderFilename)
{
	if (!InitialiseShaders(renderer, shaderFilename))
	{
		return false;
	}

	if (!InitialiseBuffers(renderer))
	{
		return false;
	}
	return true;
}

bool ParticleSystem::InitialiseShaders(Direct3D* renderer, LPCWSTR shaderFilename)
{
	ID3DBlob* vertexShaderBlob = NULL;
	ID3DBlob* geometryShaderBlob = NULL;
	ID3DBlob* pixelShaderBlob = NULL;
	ID3DBlob* errorBlob = NULL;

	const unsigned int numberOfVertexElements = 3;
	D3D11_INPUT_ELEMENT_DESC vertexLayout[numberOfVertexElements];

	if (FAILED(D3DCompileFromFile(shaderFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VSMain", "vs_4_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &vertexShaderBlob, &errorBlob)))
	{
		if (errorBlob)
		{
			OutputDebugString((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		return false;
	}

	if (FAILED(D3DCompileFromFile(shaderFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "GSMain", "gs_4_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &geometryShaderBlob, &errorBlob)))
	{
		if (errorBlob)
		{
			OutputDebugString((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}

		if (vertexShaderBlob)
			vertexShaderBlob->Release();

		return false;
	}

	if (FAILED(D3DCompileFromFile(shaderFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PSMain", "ps_4_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pixelShaderBlob, &errorBlob)))
	{
		if (errorBlob)
		{
			OutputDebugString((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}

		if (vertexShaderBlob)
			vertexShaderBlob->Release();
		if (geometryShaderBlob)
			geometryShaderBlob->Release();

		return false;
	}

	if (FAILED(renderer->GetDevice()->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), NULL, &s_vertexShader)))
	{
		if (vertexShaderBlob)
			vertexShaderBlob->Release();
		if (geometryShaderBlob)
			geometryShaderBlob->Release();
		if (pixelShaderBlob)
			pixelShaderBlob->Release();
		if (errorBlob)
			errorBlob->Release();

		return false;
	}
	if (FAILED(renderer->GetDevice()->CreateGeometryShader(geometryShaderBlob->GetBufferPointer(), geometryShaderBlob->GetBufferSize(), NULL, &s_geometryShader)))
	{
		if (vertexShaderBlob)
			vertexShaderBlob->Release();
		if (geometryShaderBlob)
			geometryShaderBlob->Release();
		if (pixelShaderBlob)
			pixelShaderBlob->Release();
		if (errorBlob)
			errorBlob->Release();

		return false;
	}
	if (FAILED(renderer->GetDevice()->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), NULL, &s_pixelShader)))
	{
		if (vertexShaderBlob)
			vertexShaderBlob->Release();
		if (geometryShaderBlob)
			geometryShaderBlob->Release();
		if (pixelShaderBlob)
			pixelShaderBlob->Release();
		if (errorBlob)
			errorBlob->Release();

		return false;
	}

	vertexLayout[0].SemanticName = "POSITION";
	vertexLayout[0].SemanticIndex = 0;
	vertexLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vertexLayout[0].InputSlot = 0;
	vertexLayout[0].AlignedByteOffset = 0;
	vertexLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vertexLayout[0].InstanceDataStepRate = 0;

	vertexLayout[1].SemanticName = "COLOR";
	vertexLayout[1].SemanticIndex = 0;
	vertexLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // It is 4 32-bit floats so our format matches that
	vertexLayout[1].InputSlot = 0;
	vertexLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// Here we would calculate the byte offset by factoring in the size of the last element
																		// however this value here tells Direct3D to calculate it for us, which is nice!
	vertexLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vertexLayout[1].InstanceDataStepRate = 0;

	vertexLayout[2].SemanticName = "TEXCOORD";
	vertexLayout[2].SemanticIndex = 0;
	vertexLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;		// A texture coord is 2 32-bit floats
	vertexLayout[2].InputSlot = 0;
	vertexLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// Here we would calculate the byte offset by factoring in the size of the last element
																		// however this value here tells Direct3D to calculate it for us, which is nice!
	vertexLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vertexLayout[2].InstanceDataStepRate = 0;

	if (FAILED(renderer->GetDevice()->CreateInputLayout(vertexLayout, numberOfVertexElements, vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &s_inputLayout)))
	{
		if (vertexShaderBlob)
			vertexShaderBlob->Release();
		if (geometryShaderBlob)
			geometryShaderBlob->Release();
		if (pixelShaderBlob)
			pixelShaderBlob->Release();
		if (errorBlob)
			errorBlob->Release();

		return false;
	}

	return true;
}

bool ParticleSystem::InitialiseBuffers(Direct3D* renderer)
{
	D3D11_BUFFER_DESC constantsBufferDescription;

	constantsBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	constantsBufferDescription.ByteWidth = sizeof(ParticleConstantsBuffer);
	constantsBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantsBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantsBufferDescription.MiscFlags = 0;
	constantsBufferDescription.StructureByteStride = 0;

	if (FAILED(renderer->GetDevice()->CreateBuffer(&constantsBufferDescription, NULL, &s_constantsBuffer)))
	{
		return false;
	}

	return true;
}

bool ParticleSystem::InitialiseSamplers(Direct3D* renderer)
{
	D3D11_SAMPLER_DESC textureSamplerDescription;

	textureSamplerDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	textureSamplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	textureSamplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	textureSamplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	textureSamplerDescription.MipLODBias = 0.0f;
	textureSamplerDescription.MaxAnisotropy = 1;
	textureSamplerDescription.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	textureSamplerDescription.BorderColor[0] = 1;
	textureSamplerDescription.BorderColor[1] = 1;
	textureSamplerDescription.BorderColor[2] = 1;
	textureSamplerDescription.BorderColor[3] = 1;
	textureSamplerDescription.MinLOD = 0;
	textureSamplerDescription.MaxLOD = D3D11_FLOAT32_MAX;

	if (FAILED(renderer->GetDevice()->CreateSamplerState(&textureSamplerDescription, &s_textureSampler)))
	{
		return false;
	}

	return false;
}

void ParticleSystem::Begin(Direct3D* renderer)
{
	renderer->GetDeviceContext()->IASetInputLayout(s_inputLayout);

	renderer->GetDeviceContext()->VSSetShader(s_vertexShader, NULL, 0);
	renderer->GetDeviceContext()->VSSetConstantBuffers(0, 1, &s_constantsBuffer);
	
	renderer->GetDeviceContext()->GSSetShader(s_geometryShader, NULL, 0);
	renderer->GetDeviceContext()->GSSetConstantBuffers(0, 1, &s_constantsBuffer);
	
	renderer->GetDeviceContext()->PSSetShader(s_pixelShader, NULL, 0);
	renderer->GetDeviceContext()->PSSetConstantBuffers(0, 1, &s_constantsBuffer);
	renderer->GetDeviceContext()->PSSetSamplers(0, 1, &s_textureSampler);

	renderer->TurnOffZWrite();
	renderer->TurnOnBlending();
}

void ParticleSystem::End(Direct3D* renderer)
{
	// Ensure we leave everything how we found it

	// If we don't reset the geometry shader, our UI won't draw
	renderer->GetDeviceContext()->GSSetShader(NULL, NULL, 0);

	renderer->TurnOnZWrite();
	renderer->TurnOffBlending();
}

void ParticleSystem::ShutdownSystem()
{
	if (s_vertexShader)
	{
		s_vertexShader->Release();
		s_vertexShader = NULL;
	}
	if (s_pixelShader)
	{
		s_pixelShader->Release();
		s_pixelShader = NULL;
	}
	if (s_geometryShader)
	{
		s_geometryShader->Release();
		s_geometryShader = NULL;
	}
	if (s_inputLayout)
	{
		s_inputLayout->Release();
		s_inputLayout = NULL;
	}
	if (s_constantsBuffer)
	{
		s_constantsBuffer->Release();
		s_constantsBuffer = NULL;
	}
	if (s_textureSampler)
	{
		s_textureSampler->Release();
		s_textureSampler = NULL;
	}
}
#include "PhongShader.h"
#include <iostream>

PhongShader::PhongShader(SceneLighting* lightingSetup, Camera* cam) : Shader()
{
	m_lightSetup = lightingSetup;
	m_camera = cam;
}

PhongShader::~PhongShader() { }

bool PhongShader::Initialise(ID3D11Device* device, LPCWSTR vertexFilename, LPCWSTR pixelFilename)
{
	if (!Shader::Initialise(device, vertexFilename, pixelFilename))		//We'll use the parent method to create most of the shader
		return false;

	if (!InitSamplers(device))
		return false;

	if (!InitBuffers(device))
		return false;

	return true;
}

bool PhongShader::InitSamplers(ID3D11Device* device)
{
	D3D11_SAMPLER_DESC textureSamplerDescription;	//When we create a sampler we need a Description struct to describe how we want to create the sampler

	//Now all we need to do is fill out our sampler description
	textureSamplerDescription.Filter = D3D11_FILTER_ANISOTROPIC;		//This is the Filtering method used for the texture... 
	//...different values will give you different quality texture output
	textureSamplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		//This defines what happens when we sample outside of the range [0...1]
	textureSamplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;		//In our case we just want it to wrap around so that we always are sampling something
	textureSamplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	textureSamplerDescription.MipLODBias = 0.0f;							//The rest of these values are really just "Defaults"
	textureSamplerDescription.MaxAnisotropy = 1;							//If you want more info look up D3D11_SAMPLER_DESC on MSDN
	textureSamplerDescription.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	textureSamplerDescription.BorderColor[0] = 1;
	textureSamplerDescription.BorderColor[1] = 1;
	textureSamplerDescription.BorderColor[2] = 1;
	textureSamplerDescription.BorderColor[3] = 1;
	textureSamplerDescription.MinLOD = 0;
	textureSamplerDescription.MaxLOD = D3D11_FLOAT32_MAX;

	//and create the sampler!
	if (FAILED(device->CreateSamplerState(&textureSamplerDescription, &m_textureSampler)))
	{
		return false;
	}

	return true;

}

bool PhongShader::InitBuffers(ID3D11Device* device)
{
	// Light Buffer
	D3D11_BUFFER_DESC lightBufferDescription;
	lightBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDescription.ByteWidth = sizeof(LightBuffer);
	lightBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDescription.MiscFlags = 0;
	lightBufferDescription.StructureByteStride = 0;

	if (FAILED(device->CreateBuffer(&lightBufferDescription, NULL, &m_lightBuffer)))
	{
		return false;
	}

	// Material Buffer
	D3D11_BUFFER_DESC materialBufferDescription;
	materialBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	materialBufferDescription.ByteWidth = sizeof(MaterialBuffer);
	materialBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	materialBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	materialBufferDescription.MiscFlags = 0;
	materialBufferDescription.StructureByteStride = 0;

	if (FAILED(device->CreateBuffer(&materialBufferDescription, NULL, &m_materialBuffer)))
	{
		return false;
	}

	return true;
}

void PhongShader::Begin(ID3D11DeviceContext* context)
{
	Shader::Begin(context);	 //Let the parent class do most of the work!

	context->PSSetSamplers(0, 1, &m_textureSampler);
	context->PSSetConstantBuffers(0, 1, &m_materialBuffer);
	context->PSSetConstantBuffers(1, 1, &m_lightBuffer);
}

bool PhongShader::SetConstants(ID3D11DeviceContext* context, Matrix world, Matrix view, Matrix projection)
{
	if (!Shader::SetConstants(context, world, view, projection))
	{
		return false;
	}

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	LightBuffer* inputData;

	if (FAILED(context->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		return false;
	}

	inputData = (LightBuffer*)mappedResource.pData;
	
	// Send each light struct into our LightBuffer
	// We send all lights even if they're disabled
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		inputData->lights[i] = *m_lightSetup->GetLight(i);
	}

	inputData->globalAmbient = m_lightSetup->GetAmbient();
	inputData->cameraPos = m_camera->GetPosition();

	context->Unmap(m_lightBuffer, 0);

	return true;
}

bool PhongShader::SetMaterial(ID3D11DeviceContext* context, Material* material)
{
	// Phong shaders do use a material so we'll send it to the GPU in here

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	if (FAILED(context->Map(m_materialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		return false;
	}

	MaterialBuffer* inputMaterial;
	inputMaterial = (MaterialBuffer*)mappedResource.pData;
	inputMaterial->material = *material;

	context->Unmap(m_materialBuffer, 0);

	return true;
}

bool PhongShader::SetTexture(ID3D11DeviceContext* context, ID3D11ShaderResourceView* textureView)
{
	//This shader does have a texture so it does something in this method!
	context->PSSetShaderResources(0, 1, &textureView);
	return true;
}

void PhongShader::Release()
{
	Shader::Release();
	if (m_textureSampler)
	{
		m_textureSampler->Release();
		m_textureSampler = NULL;
	}
	if (m_lightBuffer)
	{
		m_lightBuffer->Release();
		m_lightBuffer = NULL;
	}
	if (m_materialBuffer)
	{
		m_materialBuffer->Release();
		m_materialBuffer = NULL;
	}

}
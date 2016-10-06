#ifndef PHONGSHADER_H
#define PHONGSHADER_H

#include "Shader.h"
#include "SceneLighting.h"
#include "Camera.h"

/*
The general process of adding a new buffer is as follows

- Declare the cbuffer in the vertex and/or pixel shader (depending on where its data is needed)
- Declare a matching struct in C++ that matches the cbuffer
- This struct will need padding added so no data crosses a 16 byte boundary
- The size of the struct as a whole should also be a multiple of 16 bytes

- Both the CPU and GPU now have a common understanding of the data that will flow between them

- Declare a new ID3D11Buffer in C++
- Create the buffer inside Shader::Initialise (or one of its children)
- Bind the buffer to the shader inside Shader::Begin (or one of its children)
- Send through the actual data inside Shader::SetConstants (or one of its children)
- here we use map to upload the data in the GPU to system memory
- then we play with it and set all the appropriate values
- calling unmap copies it back to the GPU
- we can now access the data in our shaders using the varibles declared in our cbuffers
*/


class PhongShader : public Shader
{
private:
	bool InitSamplers(ID3D11Device* device);
	bool InitBuffers(ID3D11Device* device);

protected:
	struct MaterialBuffer
	{
		Material material;
	};
	struct LightBuffer
	{
		Light lights[MAX_LIGHTS];
		Color globalAmbient;
		Vector3 cameraPos;
		float padding;
	};

	SceneLighting* m_lightSetup;
	Camera* m_camera;

	ID3D11Buffer* m_lightBuffer;
	ID3D11Buffer* m_materialBuffer;

	ID3D11SamplerState* m_textureSampler;

public:
	PhongShader(SceneLighting* lightSetup, Camera* cam);
	virtual ~PhongShader();

	virtual void Begin(ID3D11DeviceContext* context);
	virtual bool Initialise(ID3D11Device* device, LPCWSTR vertexFilename, LPCWSTR pixelFilename);
	virtual bool SetConstants(ID3D11DeviceContext* context, Matrix world, Matrix view, Matrix projection);
	virtual bool SetMaterial(ID3D11DeviceContext* context, Material* material);
	virtual bool SetTexture(ID3D11DeviceContext* context, ID3D11ShaderResourceView* textureView);
	virtual void Release();

};

#endif
/*	FIT2049 - Example Code
*	Light.h
*	Created by Mike Yeates - 2016 - Monash University
*/

#ifndef LIGHT_H
#define LIGHT_H

#include <d3d11.h>
#include "DirectXTK/SimpleMath.h"

using namespace DirectX::SimpleMath;

enum LightType
{
	DirectionalLight = 0,
	PointLight = 1,
	SpotLight = 2
};

class Light
{
private:
	LightType m_lightType;
	Vector3 m_position;
	//----------------------------------- (16 byte boundary)
	Vector3 m_direction;
	// Remember how "enabled" is a bool in the shader?
	// A bool is only 1 byte in C and 4 in HLSL, so to maintain allignment
	// we need to use an int here
	int m_enabled;
	//----------------------------------- (16 byte boundary)
	Color m_color;
	//----------------------------------- (16 byte boundary)
	float m_spotAngle;
	float m_constantAttenuation;
	float m_linearAttenuation;
	float m_quadraticAttenuation;
	//----------------------------------- (16 byte boundary)

public:
	Light()
	{
		m_position = Vector3::Zero;
		m_direction = Vector3::Zero;
		m_color = Color(0, 0, 0, 0);
		m_spotAngle = 0.0f;
		m_constantAttenuation = 0.0f;
		m_linearAttenuation = 0.0f;
		m_quadraticAttenuation = 0.0f;
		m_lightType = DirectionalLight;
		m_enabled = false;
	}

	// Accessors
	LightType GetLightType() { return m_lightType; }
	Vector3 GetPosition() { return m_position; }
	Vector3 GetDirection() { return m_direction; }
	bool IsEnabled() { return m_enabled == 0 ? false : true; }
	Color GetColour() { m_color; }
	float GetSpotAngle() { return m_spotAngle; }
	float GetConstantAttenuation() { return m_constantAttenuation; }
	float GetLinearAttenuation() { return m_linearAttenuation; }
	float GetQuadraticAttenuation() { return m_quadraticAttenuation; }

	// Mutators
	void SetLightType(LightType type) { m_lightType = type; }
	void SetPosition(Vector3 pos) { m_position = pos; }
	void SetDirection(Vector3 direction)
	{
		// Direction vector must be normalized for consistent lighting results
		direction.Normalize();
		m_direction = direction;
	}
	void Enable() { m_enabled = true; }
	void Disable() { m_enabled = false; }
	void SetColour(Color colour) { m_color = colour; }
	void SetSpotAngle(float angle) { m_spotAngle = angle; }
	void SetConstantAttenuation(float atten) { m_constantAttenuation = atten; }
	void SetLinearAttenuation(float atten) { m_linearAttenuation = atten; }
	void SetQuadraticAttenuation(float atten) { m_quadraticAttenuation = atten; }
};

#endif

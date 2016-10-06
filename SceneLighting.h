/*	FIT2049 - Example Code
*	SceneLighting.h
*	Created by Mike Yeates - 2016 - Monash University
*	Responsible for creating lights and managing a collection of them
*/

#ifndef SCENELIGHTING_H
#define SCENELIGHTING_H

#include <d3d11.h>
#include <vector>
#include "DirectXTK/SimpleMath.h"
#include "Light.h"

#define MAX_LIGHTS 8

using namespace DirectX::SimpleMath;

class SceneLighting
{
private:
	Light* m_lights[MAX_LIGHTS];
	int m_nextFreeLight;

	// Ambient light is everywhere. We also have a per-material ambient if you want to
	// cancel this out for a particular object
	Color m_ambient;

	void InitLights();

public:
	SceneLighting();
	SceneLighting(Color ambientLight);
	~SceneLighting();

	Light* AddDirectionalLight(Vector3 orientation, Color color);
	Light* AddPointLight(Vector3 position, Color color, float constantAttenuation, float linearAttenuation, float quadraticAttenuation);
	Light* AddSpotLight(Vector3 position, Vector3 orientation, Color color, float spotAngle, float constantAttenuation, float linearAttenuation, float quadraticAttenuation);

	void SetAmbient(Color ambient);
	Color GetAmbient();

	// Our shader uses this to funnel light data into its buffer
	Light* GetLight(int i);

};

#endif
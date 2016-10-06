#include "SceneLighting.h"

SceneLighting::SceneLighting()
{
	m_ambient = Color(0.3f, 0.3f, 0.3f, 1.0f);
	InitLights();
}

SceneLighting::SceneLighting(Color ambient)
{
	m_ambient = ambient;
	InitLights();
}

SceneLighting::~SceneLighting()
{
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		delete m_lights[i];
		m_lights[i] = NULL;
	}
}

void SceneLighting::InitLights()
{
	m_nextFreeLight = -1;

	// Create some disabled lights so our light buffer is full of valid data
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		m_lights[i] = new Light();
	}
}

Light* SceneLighting::AddDirectionalLight(Vector3 orientation, Color color)
{
	if (m_nextFreeLight + 1 >= MAX_LIGHTS)
		return NULL;

	m_nextFreeLight++;
	m_lights[m_nextFreeLight]->Enable();
	m_lights[m_nextFreeLight]->SetLightType(DirectionalLight);
	m_lights[m_nextFreeLight]->SetDirection(orientation);
	m_lights[m_nextFreeLight]->SetColour(color);

	return m_lights[m_nextFreeLight];
}

Light* SceneLighting::AddPointLight(Vector3 position, Color color, float constantAttenuation, float linearAttenuation, float quadraticAttenuation)
{
	if (m_nextFreeLight + 1 >= MAX_LIGHTS)
		return NULL;

	m_nextFreeLight++;
	m_lights[m_nextFreeLight]->Enable();
	m_lights[m_nextFreeLight]->SetLightType(PointLight);
	m_lights[m_nextFreeLight]->SetPosition(position);
	m_lights[m_nextFreeLight]->SetColour(color);
	m_lights[m_nextFreeLight]->SetConstantAttenuation(constantAttenuation);
	m_lights[m_nextFreeLight]->SetLinearAttenuation(linearAttenuation);
	m_lights[m_nextFreeLight]->SetQuadraticAttenuation(quadraticAttenuation);

	return m_lights[m_nextFreeLight];
}

Light* SceneLighting::AddSpotLight(Vector3 position, Vector3 orientation, Color color, float spotAngle, float constantAttenuation, float linearAttenuation, float quadraticAttenuation)
{
	if (m_nextFreeLight + 1 >= MAX_LIGHTS)
		return NULL;

	m_nextFreeLight++;
	m_lights[m_nextFreeLight]->Enable();;
	m_lights[m_nextFreeLight]->SetLightType(SpotLight);
	m_lights[m_nextFreeLight]->SetPosition(position);
	m_lights[m_nextFreeLight]->SetDirection(orientation);
	m_lights[m_nextFreeLight]->SetColour(color);
	m_lights[m_nextFreeLight]->SetSpotAngle(spotAngle);
	m_lights[m_nextFreeLight]->SetConstantAttenuation(constantAttenuation);
	m_lights[m_nextFreeLight]->SetLinearAttenuation(linearAttenuation);
	m_lights[m_nextFreeLight]->SetQuadraticAttenuation(quadraticAttenuation);

	return m_lights[m_nextFreeLight];
}

Light* SceneLighting::GetLight(int i)
{
	return m_lights[i];
}

Color SceneLighting::GetAmbient()
{
	return m_ambient;
}

void SceneLighting::SetAmbient(Color ambient)
{
	m_ambient = ambient;
}


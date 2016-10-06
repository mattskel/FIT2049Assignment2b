#include "ItemBox.h"
#include "Kart.h"

#include <iostream>

ItemBox::ItemBox(Mesh* mesh, Shader* shader, Texture* texture, Material* mat, Vector3 position) :
	SpinningObject(mesh, shader, texture, position, mat, 8.0f)
{
	m_boundingBox = CBoundingBox(GetPosition() + m_mesh->GetMin(), GetPosition() + m_mesh->GetMax());
	recentCollision = false;
	itemValue = std::rand() % 5;
	m_timeStart = 0;
	m_badBox = false;
}

void ItemBox::Update(float timestep)
{
	// TODO move bounds
	if (!m_badBox) {
		// Check the time since it was hit
		// If lesss than some predefined time set to 0
		if (time(NULL) - m_timeStart < 3) {
			SetUniformScale(m_tmpUniformScale);
		}
		// Rescale and spin the ItemBox
		else if (m_tmpUniformScale < 1.0) {
			m_rotY += m_spinningSpeed * timestep;
			m_tmpUniformScale += 0.01;
			SetUniformScale(m_tmpUniformScale);
		}
	}
}

void ItemBox::OnKartCollisionEnter(Kart* other)
{
	// Give the value of the box to the kart
	// set the size to 0
	// Generate a new item value
	// Set the start time
	if (!m_badBox) {
		if (time(NULL) - m_timeStart > 5) {
			//OutputDebugString("OnKartCollisionEnter\n");
			other->SetItemValue(itemValue);
			m_tmpUniformScale = 0.0;
			itemValue = std::rand() % 5;
			m_timeStart = time(NULL);
		}
	}
	// If it is a bad box then the status is set to 0
	else {
		m_status = 0;
	}
	//OutputDebugString("ItemBox Collision Enter\n");
}

void ItemBox::OnKartCollisionStay(Kart* other)
{
	//OutputDebugString("ItemBox Collision Stay\n");
}

void ItemBox::OnKartCollisionExit(Kart* other)
{
	//OutputDebugString("ItemBox Collision Exit\n");
}
#ifndef KART_H
#define KART_H

#include "PhysicsObject.h"
#include "InputController.h"
#include "Light.h"
#include "ItemBox.h"
#include "Wall.h"
#include "MovingItemObject.h"
#include "Shell.h"
#include "Balloon.h"
//#include "RedShell.h"

#include <vector>

// Forward declare ItemBox
class ItemBox;
class MovingItemObject;
class Shell;

class Kart : public PhysicsObject
{
private:
	InputController* m_input;
protected:
	float m_moveSpeed;
	float m_turnSpeed;

	CBoundingBox m_boundingBox;

	int m_livesRemaining;	// The number of lives currently held by the Kart
	boolean m_invincible;	// Used to check if the Kart has used a star
	int m_invincibleStart;	// start time to check how long the star has been running

	int m_itemValue;	// The value of the item currently held
	//int m_itemReleased;

	int m_gameObjectIndex;

	// Kart will have pointers to everything required to build its own Items
	std::vector<const char*>* m_itemTextures;
	std::vector<const char*>* m_itemMeshes;
	const char* m_balloonTexture;
	Material* m_itemMaterial;
	const char* m_balloonMesh;
	Shader* m_texturedShader;

	ItemBox* m_previousItemBox;		// To make sure we don't return to the previous ItemBox, for enemies

	// Kart needs to store the items it creates somewhere
	std::vector<GameObject*>* m_gameObjects;
	std::vector<Kart*>* m_karts;	// Need a pointer to other karts for RedShell
	std::vector<Shell*>* m_shells;
	std::vector<GameObject*>* m_otherItems;
	std::vector<Balloon*> m_balloons;
	std::vector<ItemBox*>* m_itemBoxes;

public:
	Kart(Mesh* mesh,
		Shader* shader,
		Texture* texture,
		Material* mat,
		Vector3 position,
		InputController* input);

	void InitBalloons();

	void Update(float timestep);

	CBoundingBox GetBounds() { return m_boundingBox; }

	Vector3 GetLocalForward();

	void SetItemValue(int item) { m_itemValue = item; }
	int GetItemValue() { return m_itemValue; }

	// Gives Kart all the relevant pointers for construction an item
	void SetItemPointers(std::vector<const char*>* itemTextures,
		std::vector<const char*>* itemMeshes,
		Shader* texturedShader);

	// So the kart can draw its own balloons
	void SetBalloonPointers(const char* balloonTexture, const char* balloonMesh);

	// Requires this information so the kart can put items onto the appropriate lists
	void SetObjects(std::vector<GameObject*>* gameObjects,
		std::vector<Kart*>* karts,
		std::vector<Shell*>* shells,
		std::vector<GameObject*>* otherItems);

	// Sets the position of the kart in the gameObject List
	void SetGameObjectIndex(int gameObjectIndex) { m_gameObjectIndex = gameObjectIndex; }
	// Sets a pointer to the item box array
	// Need this so the kart can put bad boxes on the item box array
	void SetItemBoxes(std::vector<ItemBox*>* itemBoxes) { m_itemBoxes = itemBoxes; }
	int GetGameObjectIndex() { return m_gameObjectIndex; }
	boolean GetInvincibility() { return m_invincible; }

	// Kart executes this when an object is fired
	// Constructs the item object and then adds it to the item lists
	void ItemReleased();

	void LifeLost();

	void OnKartCollisionEnter(Kart* other);
	void OnKartCollisionStay(Kart* other);
	void OnKartCollisionExit(Kart* other);

	void OnItemCollisionEnter(ItemBox* other);
	void OnItemCollisionStay(ItemBox* other);
	void OnItemCollisionExit(ItemBox* other);

	void OnWallCollisionEnter(Wall* other);
	void OnWallCollisionStay(Wall* other);
	void OnWallCollisionExit(Wall* other);

	//void OnItemObjectCollisionEnter(MovingItemObject* other);

	void OnShellCollisionEnter(Shell* other);
	void OnOtherItemCollisionEnter();

};

#endif

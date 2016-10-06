/*	FIT2049 - Example Code
*	Game.h
*	Created by Elliott Wilson & Mike Yeates - 2016 - Monash University
*	This class is the heart of our game and is also where our game logic will reside
*	It contains the overall Update and Render method for the whole game
*	You should eventually split some game logic into other objects which Game will manage
*	Watch the size of this class - it can grow out of control very fast!
*/

#ifndef GAME_H
#define GAME_H

#include "Direct3D.h"
#include "InputController.h"
#include "GameObject.h"
#include "StateMachine.h"
#include "Mesh.h"
#include "Camera.h"
#include "Button.h"
#include "Shader.h"
#include "DirectXTK/SpriteBatch.h"
#include "DirectXTK/SpriteFont.h"
#include "CollisionManager.h"
#include "SceneLighting.h"
#include "Kart.h"
#include "ItemBox.h"
#include "EnemyKart.h"
#include "Wall.h"
#include "MovingItemObject.h"
#include "Shell.h"
#include "Balloon.h"
#include "ParticleSystem.h"

#include <vector>

class Game
{
private:
	// All the states our game can be in. The STATE_COUNT down the bottom is just an easy way
	// for us to count how many states this enum has (cast this to an int)
	enum class GameStates
	{
		NO_STATE,
		MENU_STATE,
		GAMEPLAY_STATE,
		PAUSE_STATE,
		GAMEOVER_STATE,
		STATE_COUNT
	};

	// Our state machine is a generic class - we need to tell it what types it manages
	// It knows about two things - our states, and also who ownes those states
	StateMachine<GameStates, Game>* m_stateMachine;

	Direct3D* m_renderer;
	Camera* m_currentCam;		
	InputController* m_input;
	CollisionManager* m_collisionManager;

	std::vector<GameObject*> m_gameObjects;
	std::vector<Kart*> m_karts;
	std::vector<ItemBox*> m_itemBoxes;
	std::vector<Wall*> m_walls;

	std::vector<ParticleSystem*> m_particleSystems;

	Kart* m_playerKart;

	boolean m_gameOver;
	boolean m_newGame;
	boolean m_playerWin;

	// Items
	std::vector<MovingItemObject*> m_movingItemObjects;
	std::vector<Shell*> m_shells;
	std::vector<GameObject*> m_otherItems; // Bananas and BadItems
	Texture* m_currentItemSprite;
	const char* m_currentItem;

	// Sprites / Fonts
	SpriteBatch* m_spriteBatch;
	SpriteFont* m_arialFont12;
	SpriteFont* m_arialFont18;

	// Shaders
	SceneLighting* m_lightingController;
	Shader* m_unlitShader;
	Shader* m_texturedShader;
	Shader* m_phongShader;

	// Materials
	Material* m_shinyMetal;
	Material* m_shinyPlastic;
	Material* m_matte;

	// The menu screen will display two buttons
	Button* m_startButton;
	Button* m_quitButton;

	// Array to hold icon sprites
	std::vector<const char*> m_currentItemArray;
	std::vector<const char*> m_itemList;
	std::vector<const char*> m_itemTextures;
	std::vector<const char*> m_itemMeshes;
	std::vector<const char*> m_kartTextures;

	// Splitting initialisation up into several steps
	void InitStates();
	void InitMaterials();
	void InitLights();
	bool InitShaders();
	bool InitParticleSystems();
	bool LoadTextures();
	bool LoadMeshes();
	void LoadFonts();
	void InitGameWorld();

	// UI drawing helpers
	void InitUI();
	void DrawGameUI();
	void DrawMenuUI();
	void DrawPauseUI();
	void DrawGameOverUI();

	// Every state in our game will have four callbacks
	// We register these with the StateMachine and it calls them for us
	void Gameplay_OnEnter();
	void Gameplay_OnUpdate(float timestep);
	void Gameplay_OnRender();
	void Gameplay_OnExit();

	// I have added these two additional states
	// These are from Lab 5
	void Menu_OnEnter();
	void Menu_OnUpdate(float timestep);
	void Menu_OnRender();
	void Menu_OnExit();

	void Pause_OnEnter();
	void Pause_OnUpdate(float timestep);
	void Pause_OnRender();
	void Pause_OnExit();

	void GameOver_OnEnter();
	void GameOver_OnUpdate(float timestep);
	void GameOver_OnRender();
	void GameOver_OnExit();

public:
	Game();	
	~Game();

	bool Initialise(Direct3D* renderer, InputController* input); //The initialise method loads all of the content for the game (meshes, textures, etc.)

	void Update(float timestep);	//The overall Update method for the game. All gameplay logic will be done somewhere within this method
	void Render();					//The overall Render method for the game. Here all of the meshes that need to be drawn will be drawn

	void Shutdown(); //Cleanup everything we initialised
};

#endif
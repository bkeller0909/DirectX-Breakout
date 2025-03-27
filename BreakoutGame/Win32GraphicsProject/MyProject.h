#ifndef _MyProject_h
#define _MyProject_h
#include  <CommonStates.h>
#include "DirectX.h"
#include "Font.h"
#include "TextureType.h"
#include "Sprite.h"
#include "Collision2D.h"
#include "Block.h"
#include "Ball.h"
#include "SuperBlock.h"
#include "Paddle.h"

//GAME 1201 Term Assignment 1

// forward declare the sprite batch
namespace DirectX { class SpriteBatch; };

enum GameState { MAIN_MENU, GAME, END_SCREEN };


//----------------------------------------------------------------------------------------------
// Main project class
//	Inherits the directx class to help us initalize directX
//----------------------------------------------------------------------------------------------

class MyProject : public DirectXClass
{
public:
	// constructor
	MyProject(HINSTANCE hInstance);
	~MyProject();

	// Initialize any Textures we need to use
	void InitializeTextures();

	// window message handler
	LRESULT ProcessWindowMessages(UINT msg, WPARAM wParam, LPARAM lParam);

	// Called by the render loop to render a single frame
	void Render(void);

	void MainMenuRender();
	void GameScreenRender();
	void EndScreenRender();

	// Called by directX framework to allow you to update any scene objects
	void Update(float deltaTime);

	void MainMenuUpdate(float deltaTime);
	void GameScreenUpdate(float deltaTime);
	void EndScreenUpdate(float deltaTime);

private:	
	GameState currentState;
	
	int score;
	static const int NUM_ROWS = 8;
	static const int NUM_COLUMNS = 10;
	static const int PADDLE_SPEED = 400;

	// textures and sprites go here
	TextureType background;
	TextureType ballTexture;
	TextureType blockTexture;
	TextureType paddleTexture;

	// game objects
	Block block;
	Block* blocks[NUM_COLUMNS][NUM_ROWS];
	SuperBlock superBlock;
	Ball ball;
	Paddle paddle;


	// movement
	bool left = false;
	bool right = false;

	// sprite batch 
	DirectX::SpriteBatch* spriteBatch;

	DirectX::CommonStates* commonStates;

	// mouse variables
	Vector2 mousePos;
	bool buttonDown;

	// call when the mouse is released
	void OnMouseDown();

};

#endif
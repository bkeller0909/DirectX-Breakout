// Button art reference: http://opengameart.org/content/ui-pack (public domain)
//

#include "MyProject.h"
#include <Windowsx.h> // for GET__LPARAM macros
#include <SpriteBatch.h>
#include <d3d11.h>
#include <SimpleMath.h>
#include <DirectXColors.h>
#include <sstream>
#include "Collision2D.h"

/*
* Midterm Assignment: Breakout
* 
* Name: Brandon Keller
* Date: November 17, 2023
* 
* Course: GAME1201 - Computer Graphics Programming
*/

using namespace DirectX;
using namespace DirectX::SimpleMath;

// helper function
//
// returns a random float between 0 & 1
float RandFloat() { return float(rand())/float(RAND_MAX); } 

//----------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd)
{
	MyProject application(hInstance);    // Create the class variable

	if( application.InitWindowsApp(L"BREAKOUT!", nShowCmd) == false )    // Initialize the window, if all is well show and update it so it displays
	{
		return 0;                   // Error creating the window, terminate application
	}

	if( application.InitializeDirect3D() )
	{
		application.SetDepthStencil(true);      // Tell DirectX class to create and maintain a depth stencil buffer
		application.InitializeTextures();
		application.MessageLoop();				// Window has been successfully created, start the application message loop
	}

	return 0;
}

//----------------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------------
MyProject::MyProject(HINSTANCE hInstance)
	: DirectXClass(hInstance)
{
	mousePos = Vector2(clientWidth * 0.5f, clientHeight * 0.5f);
	buttonDown = false;
	spriteBatch = NULL;
	
	ClearColor = Color(DirectX::Colors::DarkGray.v);
}

//----------------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------------
MyProject::~MyProject()
{
	delete spriteBatch;
}

//----------------------------------------------------------------------------------------------
// Initialize any fonts we need to use here
//----------------------------------------------------------------------------------------------
void MyProject::InitializeTextures()
{
	// initialize the sprite batch
	spriteBatch = new DirectX::SpriteBatch( DeviceContext );

	background.Load(D3DDevice, L"..\\Textures\\starfield.dds");
	ballTexture.Load(D3DDevice, L"..\\Textures\\sphere-04.png");
	paddleTexture.Load(D3DDevice, L"..\\Textures\\paddle.png");
	blockTexture.Load(D3DDevice, L"..\\Textures\\block_purple.png");
	paddleTexture.Load(D3DDevice, L"..\\Textures\\paddle.png");

	ball.Initialize(&ballTexture, Vector2(clientWidth/2, 550), 0, 0.25, Color(1, 1, 1, 1), 0);
	ball.SetVelocity(Vector2(200, 200));

	paddle.Initialize(&paddleTexture, Vector2(clientWidth/2, 650), 0, 0.35, Color(1, 1, 0, 1), 0);

	for (int i = 0; i < NUM_COLUMNS; i++) // generates columns for the blocks
	{
		for (int j = 0; j < NUM_ROWS; j++) // generates rows for the blocks
		{
			int superBlockIndex = RandFloat() * 99 + 1; // random number between 1 and 100
			if (superBlockIndex > 80) // 20% chance to get super block
			{
				// added 50 to i in the Vector2 so the blocks don't start off the screen
				blocks[i][j] = new SuperBlock();
				blocks[i][j]->Initialize(&blockTexture, Vector2((clientWidth / NUM_COLUMNS) * i + 50, (100 + 50 * j)), 0, 0.20, Color(1, 0, 0, 1), 0);
			}
			else
			{
				blocks[i][j] = new Block();
				blocks[i][j]->Initialize(&blockTexture, Vector2((clientWidth / NUM_COLUMNS) * i + 50, (100 + 50 * j)), 0, 0.20, Color(1, 1, 1, 1), 0);
			}
		}
	}

	commonStates = new CommonStates(D3DDevice);
}

//----------------------------------------------------------------------------------------------
// Window message handler
//----------------------------------------------------------------------------------------------
LRESULT MyProject::ProcessWindowMessages(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg )
	{
	case WM_MOUSEMOVE:
		mousePos.x = (float) GET_X_LPARAM(lParam);
		mousePos.y = (float) GET_Y_LPARAM(lParam);
		return 0;
	case WM_LBUTTONUP:
		buttonDown = false;
		mousePos.x = (float) GET_X_LPARAM(lParam);
		mousePos.y = (float) GET_Y_LPARAM(lParam);
		break;
	case WM_LBUTTONDOWN:
		buttonDown = true;
		mousePos.x = (float) GET_X_LPARAM(lParam);
		mousePos.y = (float) GET_Y_LPARAM(lParam);
		OnMouseDown();
		break;
	case WM_KEYUP:
		if (wParam >= '0' && wParam <= '4')
		{
			PresentInterval = wParam - '0';
		}
		// turn off movement when these keys are released
		if (wParam == 'A') { 
			left = false;
		}
		if (wParam == 'D') {
			right = false;
		}
		break;
	case WM_KEYDOWN:
		// turn on movement when these keys are pressed
		if (wParam == 'A') { 
			left = true;
		}
		if (wParam == 'D') {
			right = true;
		}
		break;
	}

	// let the base class handle remaining messages
	return DirectXClass::ProcessWindowMessages(msg, wParam, lParam);
}

//----------------------------------------------------------------------------------------------
// Called by the render loop to render a single frame
//----------------------------------------------------------------------------------------------
void MyProject::Render(void)
{
	switch (currentState)
	{
	case GameState::MAIN_MENU:
		MainMenuRender();
		break;
	case GameState::GAME:
		GameScreenRender();
		break;
	case GameState::END_SCREEN:
		EndScreenRender();
		break;

	}

	// render the base class
	DirectXClass::Render();
}

void MyProject::MainMenuRender()
{
	background.Draw(DeviceContext, BackBuffer, 0, 0);
	font.PrintMessage(0, 30, L"Main Menu", Colors::Red.v);
	font.PrintMessage(clientWidth / 2 - 50, clientHeight / 2, L"BREAKOUT", Colors::Orange.v);
	font.PrintMessage(clientWidth / 2 - 75, clientHeight / 2 + 50, L"Left Click To Start", Colors::Orange.v);
}

void MyProject::GameScreenRender()
{
	background.Draw(DeviceContext, BackBuffer, 0, 0);
	font.PrintMessage(0, 30, L"Game Menu", Colors::Red.v);

	// draw your stuff here
	spriteBatch->Begin(SpriteSortMode_BackToFront, commonStates->NonPremultiplied());

	block.Draw(spriteBatch);
	superBlock.Draw(spriteBatch);
	ball.Draw(spriteBatch);
	paddle.Draw(spriteBatch);

	// draw the arry of columns and rows of blocks
	for (int i = 0; i < NUM_COLUMNS; i++)
	{
		for (int j = 0; j < NUM_ROWS; j++)
		{
			blocks[i][j]->Draw(spriteBatch);
		}
	}


	spriteBatch->End();

	font.PrintMessage(0, 60, L"SCORE: " + to_wstring(score), Color(1, 0, 0, 1));
}

void MyProject::EndScreenRender()
{
	background.Draw(DeviceContext, BackBuffer, 0, 0);
	font.PrintMessage(0, 30, L"End Menu", Colors::Red.v);
	font.PrintMessage(clientWidth/2 - 50, clientHeight/2, L"GAME OVER", Colors::Red.v);

	spriteBatch->Begin(SpriteSortMode_BackToFront, commonStates->NonPremultiplied());
	spriteBatch->End();

	font.PrintMessage(0, 60, L"SCORE: " + to_wstring(score), Color(1, 0, 0, 1));
}

//----------------------------------------------------------------------------------------------
// Called every frame to update objects.
//	deltaTime: how much time in seconds has elapsed since the last frame
//----------------------------------------------------------------------------------------------
void MyProject::Update(float deltaTime)
{
	switch (currentState)
	{
	case GameState::MAIN_MENU:
		MainMenuUpdate(deltaTime);
		break;
	case GameState::GAME:
		GameScreenUpdate(deltaTime);
		break;
	case GameState::END_SCREEN:
		EndScreenUpdate(deltaTime);
		break;
	}
}

void MyProject::MainMenuUpdate(float deltaTime)
{
	
}

void MyProject::GameScreenUpdate(float deltaTime)
{
	ball.MoveBall(deltaTime);
	
	// Check collision with left and right borders
	if (ball.GetPosition().x <= 0 + ball.GetExtents().x || ball.GetPosition().x >= clientWidth - ball.GetExtents().x) {
		ball.SetVelocity(Vector2(-ball.GetVelocity().x, ball.GetVelocity().y));  // Reverse the horizontal direction
	}

	// collision for top and bottom
	if (ball.GetPosition().y <= 0 + ball.GetExtents().y) {
		ball.SetVelocity(Vector2(ball.GetVelocity().x, -ball.GetVelocity().y));  // Reverse the vertical direction
	}

	if (ball.GetPosition().y >= clientHeight - ball.GetExtents().x) {
		currentState = END_SCREEN;
	}

	// paddle collision with left side border of screen
	if (paddle.GetPosition().x >= 0 + paddle.GetExtents().x) {
		// paddle movement, to the left
		if (left == true) {
			paddle.SetPosition(Vector2(paddle.GetPosition().x - (PADDLE_SPEED * deltaTime), paddle.GetPosition().y));
			paddle.SetCollider(Box2D(paddle.GetPosition(), paddle.GetExtents()));
		}
	}

	// paddle collision with the right side border of screen
	if (paddle.GetPosition().x <= clientWidth - paddle.GetExtents().x) {
		// paddle movement, to the right
		if (right == true) {
			paddle.SetPosition(Vector2(paddle.GetPosition().x + (PADDLE_SPEED * deltaTime), paddle.GetPosition().y)); 
			paddle.SetCollider(Box2D(paddle.GetPosition(), paddle.GetExtents()));
		}
	}

	// block collider
	if (Collision2D::BoxCircleCheck(block.GetCollider(), ball.GetCollider())) {
		block.DoCollision(score, ball, block, deltaTime);
	}

	// paddle collider
	if (Collision2D::BoxCircleCheck(paddle.GetCollider(), ball.GetCollider())) {
		paddle.DoCollision(ball, deltaTime);
	}

	// super block collider
	if (Collision2D::BoxCircleCheck(superBlock.GetCollider(), ball.GetCollider())) {
		superBlock.DoSuperBlockCollision(ball, score, block, deltaTime);
	}

	// box and circle collision checks
	for (int i = 0; i < NUM_COLUMNS; i++)  {
		for (int j = 0; j < NUM_ROWS; j++) {
			if (Collision2D::BoxCircleCheck(blocks[i][j]->GetCollider(), ball.GetCollider())) {
				// super block check
				SuperBlock* superBlock = dynamic_cast<SuperBlock*>(blocks[i][j]);
				if (superBlock != nullptr) {
					// super block collision
					superBlock->DoSuperBlockCollision(ball, score, block, deltaTime);
				}
				else {
					// normal block collision
					blocks[i][j]->DoCollision(score, ball, block, deltaTime);
				}
			}
		}
	}
}

void MyProject::EndScreenUpdate(float deltaTime)
{

}

//----------------------------------------------------------------------------------------------
// Called when the mouse is released
//----------------------------------------------------------------------------------------------
void MyProject::OnMouseDown()
{
	// this is called when the left mouse button is clicked
	// mouse position is stored in mousePos variable
	if (currentState == MAIN_MENU) {
		currentState = GAME;
	}
	else if (currentState == GAME) {
		currentState = END_SCREEN;
	}
	else if (currentState == END_SCREEN) {
		currentState = MAIN_MENU;
	}
}
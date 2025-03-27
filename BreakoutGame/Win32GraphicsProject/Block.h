#pragma once
#include "Sprite.h"
#include "Collision2D.h"
#include "Ball.h"
#include "Block.h"

class Block : public Sprite
{
public:
	Block();
	virtual ~Block();

	void SetScoreValue(int value) { scoreValue = value; }
	int GetScoreValue() { return scoreValue; }

	void SetCollider(Box2D box) { collider = box; }
	Box2D GetCollider() { return collider; }

	void SetEnabled(bool value) { enabled = value; }
	bool GetEnabled() { return enabled; }

	void DoCollision(int& score, Ball& ball, Block& block, float& deltaTime);

	//Overloads
	void Draw(SpriteBatch* spriteBatch);
	void Initialize(TextureType* pTexture, Vector2 position, float rotinDegrees, float scale, Color color, float layer);


private:
	int scoreValue;
	bool enabled;
	Box2D collider;


};


#pragma once
#include "Sprite.h"
#include "Collision2D.h"
#include "Ball.h"

class Paddle : public Sprite
{
public:
	Paddle();
	virtual ~Paddle();

	void SetCollider(Box2D box) { collider = box; }
	Box2D GetCollider() { return collider; }

	void SetEnabled(bool value) { enabled = value; }
	bool GetEnabled() { return enabled; }

	void DoCollision(Ball& ball, float deltaTime);

	//Overloads
	void Draw(SpriteBatch* spriteBatch);
	void Initialize(TextureType* pTexture, Vector2 position, float rotinDegrees, float scale, Color color, float layer);


private:
	bool enabled;
	Box2D collider;
};


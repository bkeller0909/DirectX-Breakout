#pragma once
#include "Sprite.h"
#include "Collision2D.h"


class Ball : public Sprite
{
public:
	Ball();
	~Ball();

	void SetVelocity(Vector2 vel) { velocity = vel; }
	Vector2 GetVelocity() { return velocity; }

	void SetCollider(Circle circle) { collider = circle; }
	Circle GetCollider() { return collider; }

	//overloads
	void Initialize(TextureType* pTexture, Vector2 position, float rotinDegrees, float scale, Color color, float layer);

	void MoveBall(float deltaTime);


private:
	Circle collider;
	Vector2 velocity;
};
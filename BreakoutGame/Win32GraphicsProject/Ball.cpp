#include "Ball.h"

Ball::Ball() : Sprite()
{
}

Ball::~Ball()
{
}

void Ball::Initialize(TextureType* pTexture, Vector2 position, float rotinDegrees, float scale, Color color, float layer)
{
	Sprite::Initialize(pTexture, position, rotinDegrees, scale, color, layer);
	collider.center = position;
	collider.radius = GetWidth() / 2.0f;
}

void Ball::MoveBall(float deltaTime)
{
	Vector2 pos = GetPosition();
	pos += velocity * deltaTime;
	SetPosition(pos);

	collider.center = pos;
}
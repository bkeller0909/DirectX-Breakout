#include "Paddle.h"
#include "Ball.h"

Paddle::Paddle() : Sprite(), collider()
{
	enabled = true;
}

Paddle::~Paddle()
{
}

void Paddle::Draw(SpriteBatch* spriteBatch)
{
	if (enabled) {
		Sprite::Draw(spriteBatch);
	}
}

void Paddle::Initialize(TextureType* pTexture, Vector2 position, float rotinDegrees, float scale, Color color, float layer)
{
	Sprite::Initialize(pTexture, position, rotinDegrees, scale, color, layer);
	collider.center = position;
	collider.extents = GetExtents();

}

void Paddle::DoCollision(Ball& ball, float deltaTime)
{
	ball.SetVelocity(Vector2(ball.GetVelocity().x, -ball.GetVelocity().y));
}
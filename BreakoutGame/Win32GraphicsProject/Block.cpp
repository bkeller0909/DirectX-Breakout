#include "Block.h"
#include "Ball.h"

Block::Block() : Sprite(), collider()
{
	scoreValue = 1;
	enabled = true;

}

Block::~Block()
{
}

void Block::Draw(SpriteBatch* spriteBatch)
{
	if (enabled) {
		Sprite::Draw(spriteBatch);
	}
}

void Block::Initialize(TextureType* pTexture, Vector2 position, float rotinDegrees, float scale, Color color, float layer)
{
	Sprite::Initialize(pTexture, position, rotinDegrees, scale, color, layer);
	collider.center = position;
	collider.extents = GetExtents();

}

void Block::DoCollision(int& score, Ball& ball, Block& block, float& deltaTime)
{
	if (enabled)
	{
		score += scoreValue;
		ball.SetVelocity(Vector2(ball.GetVelocity().x, -ball.GetVelocity().y));
	}
	SetEnabled(false);

}

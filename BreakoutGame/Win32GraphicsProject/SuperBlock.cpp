#include "SuperBlock.h"

SuperBlock::SuperBlock() : Block()
{
	speedMultiplier = 1.15; // multiplies ball speed
}

SuperBlock::~SuperBlock()
{
}

void SuperBlock::DoSuperBlockCollision(Ball& ball, int& score, Block& block, float deltaTime)
{
	if (GetEnabled())
	{
		Block::DoCollision(score, ball, block, deltaTime);
		Vector2 newVelocity = ball.GetVelocity();
		newVelocity *= speedMultiplier;
		ball.SetVelocity(newVelocity);
		ball.SetVelocity(Vector2(ball.GetVelocity().x, -ball.GetVelocity().y));
	}

	SetEnabled(false);
}

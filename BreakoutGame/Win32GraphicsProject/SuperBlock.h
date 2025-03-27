#pragma once
#include "Block.h"
#include "Ball.h"


class SuperBlock : public Block
{
public:
	SuperBlock();
	~SuperBlock();

	void DoSuperBlockCollision(Ball& ball, int& score, Block& block, float deltaTime);

private:

	float speedMultiplier;
};


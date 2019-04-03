#include "Brick.h"

Brick::Brick(Object* owner)
	: Component(owner)
{
}


Brick::~Brick()
{
}

void Brick::Start() {}

void Brick::Update(float deltaTime, float totalTime) {}

void Brick::SetBrickColor(int colorIndex)
{
	if (colorIndex == 0)
	{
		color = BlueBrick;

	}
	else if (colorIndex == 1)
	{
		color = RedBrick;
	}
	else
	{
		color = YellowBrick;
	}
}

void Brick::OnPlaced() {
	//Maybe some visual effects when placed
	//GetComponent<Collider>().enabled = false;
}



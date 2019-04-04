#pragma once
#include "Component.hpp"

enum BrickColor {
	RedBrick = 1,
	YellowBrick = 10,
	BlueBrick = 100,
	Error = 0,
	Empty = 0	
};

class Brick :
	public Component
{
public:
	explicit Brick(Object* owner);
	~Brick();

	void Start() override;
	void Update(float deltaTime, float totalTime) override;

	BrickColor color;

	void SetBrickColor(int);
	void OnPlaced();

private:

};



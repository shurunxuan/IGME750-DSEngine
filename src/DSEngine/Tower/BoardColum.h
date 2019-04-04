#pragma once
#include "Component.hpp"
#include "Brick.h"

class BoardColum :
	public Component
{
public:
	explicit BoardColum(Object* owner);
	~BoardColum();

	void Start() override;
	void Update(float deltaTime, float totalTime) override;

    int index;
    bool isFull = false;

    std::vector<Brick*> bricks;
    int brickCount = 0;

    void PlaceBrick(Brick* brick);

private:
    float brickHeight;
};


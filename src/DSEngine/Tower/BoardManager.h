#pragma once
#include "Brick.h"
struct BrickMatrix {
	BrickColor bricks[];
};

class BoardManager
{
public:
	static BoardManager* getInstance();
	BoardManager();
	~BoardManager();
private:
	static BoardManager* instance;
};


#pragma once
#include "Brick.h"
#include "Scene.hpp"
#include <list>

class BrickDeck
{
public:
    BrickDeck();
    ~BrickDeck();
    static BrickDeck* getInstance();

    //Material[] Materials;
    std::list<Brick*> bricks;
    //GameObject[] brickPrefabs;

    Brick* OnDraw(Transform* transform);
    void InitDeck(Scene* scene);
    Brick* CreateBrick(Scene* scene, BrickColor color, DirectX::XMVECTOR position);

private:
    static BrickDeck* instance;

    int YellowBrickNum = 10;
    int BlueBrickNum = 10;
    int RedBrickNum = 10;
};


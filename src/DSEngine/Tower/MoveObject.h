#pragma once
#include "Component.hpp"

class MoveObject :
	public Component
{
public:
	explicit MoveObject(Object* owner);
	~MoveObject();

	void Start() override;
	void Update(float deltaTime, float totalTime) override;

	static MoveObject* instance;
private:
	
};


#pragma once
#include <list>
#include "Component.hpp"
#include "MeshRenderer.hpp"


class MoveParentObject :
	public Component
{
public:
	explicit MoveParentObject(Object* owner);
	~MoveParentObject();

	void Start() override;
	void Update(float deltaTime, float totalTime) override;

private:
	std::list<MeshRenderer*> meshRenderers;
};


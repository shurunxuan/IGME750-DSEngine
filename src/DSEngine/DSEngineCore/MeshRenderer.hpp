#pragma once
#pragma warning(disable:4251)
#include <memory>
#include "Component.hpp"
#include "Material.hpp"
#include "Mesh.hpp"

class MeshRenderer
	: public Component
{
public:
	MeshRenderer(Object* owner);
	~MeshRenderer();

	void SetMesh(const std::shared_ptr<Mesh>& m);
	void SetMaterial(const std::shared_ptr<Material>& m);

	void Start() override;
	void Update(float deltaTime, float totalTime) override;

	Material* GetMaterial() const;
	Mesh* GetMesh() const;

private:
	std::shared_ptr<Material> material;
	std::shared_ptr<Mesh> mesh;
};

inline MeshRenderer::MeshRenderer(Object* owner)
	: Component(owner)
{
	material = nullptr;
	mesh = nullptr;
}

inline MeshRenderer::~MeshRenderer()
{
}

inline void MeshRenderer::SetMesh(const std::shared_ptr<Mesh>& m)
{
	mesh = m;
}

inline void MeshRenderer::SetMaterial(const std::shared_ptr<Material>& m)
{
	material = m;
}

inline void MeshRenderer::Start()
{
}

inline void MeshRenderer::Update(float deltaTime, float totalTime)
{
}

inline Material* MeshRenderer::GetMaterial() const
{
	return material.get();
}

inline Mesh* MeshRenderer::GetMesh() const
{
	return mesh.get();
}

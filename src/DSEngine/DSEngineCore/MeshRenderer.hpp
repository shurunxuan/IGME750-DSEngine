/**
 * @file MeshRenderer.hpp
 * @author Victor Shu
 * @brief This file contains the class that represents
 * the MeshRenderer component
 * 
 * @version 0.1
 * @date 2019/03/11
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once
#pragma warning(disable:4251)
#include <memory>
#include "Component.hpp"
#include "Material.hpp"
#include "Mesh.hpp"

/**
 * @brief The mesh renderer component
 * 
 */
class MeshRenderer
	: public Component
{
public:
	/**
	 * @brief Construct a new MeshRenderer object
	 * 
	 * @param owner The object that the MeshRenderer is attached to
	 */
	MeshRenderer(Object* owner);
	/**
	 * @brief Destroy the MeshRenderer object
	 * 
	 */
	~MeshRenderer();

	/**
	 * @brief Set the Mesh of the MeshRenderer
	 * 
	 * @param m The mesh
	 */
	void SetMesh(const std::shared_ptr<Mesh>& m);
	/**
	 * @brief Set the Material of the MeshRenderer
	 * 
	 * @param m The material
	 */
	void SetMaterial(const std::shared_ptr<Material>& m);

	/**
	 * @brief Does nothing
	 * 
	 */
	void Start() override;
	/**
	 * @brief Does nothing
	 * 
	 * @param deltaTime The time that a frame costs
	 * @param totalTime The total time from the beginning of the application
	 */
	void Update(float deltaTime, float totalTime) override;

	/**
	 * @brief Get the Material of the MeshRenderer
	 * 
	 * @return Material* The pointer points to the material
	 */
	Material* GetMaterial() const;
	/**
	 * @brief Get the Mesh of the MeshRenderer
	 * 
	 * @return Mesh* The pointer points to the mesh
	 */
	Mesh* GetMesh() const;

private:
	/**
	 * @brief The material of the MeshRenderer
	 * 
	 */
	std::shared_ptr<Material> material;
	/**
	 * @brief The mesh of the MeshRenderer
	 * 
	 */
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

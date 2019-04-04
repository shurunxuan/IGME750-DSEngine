/**
 * @file Scene.hpp
 * @author Victor Shu
 * @brief This file contains the class that represents
 * the scene graph
 * 
 * @version 0.1
 * @date 2019/03/09
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once
#pragma warning(disable:4251)
#include <d3d11.h>

#include <DirectXCollision.h>

#include <list>
#include <vector>

#include <assimp/Importer.hpp> 
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Object.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "SimpleShader.hpp"
#include "MeshRenderer.hpp"
#include "PBRMaterial.hpp"

/**
 * @brief The class represents the scene graph
 * 
 */
class Scene
{
public:
	friend class DSSRendering;
	friend class DSFDirect3D;
	friend class DSSPhysics;

	/**
	 * @brief Construct a new Scene
	 * 
	 */
	Scene();
	/**
	 * @brief Destroy the Scene
	 * 
	 */
	~Scene();

	/**
	 * @brief Set the device and context
	 * 
	 * @param d Direct3D 11 device
	 * @param c Direct3D 11 device context
	 */
	void SetD3D11Device(ID3D11Device* d, ID3D11DeviceContext* c);
	/**
	 * @brief Set the Default Shaders
	 * 
	 * @param vs Default vertex shader of the scene
	 * @param ps Default pixel shader of the scene
	 */
	void SetDefaultShader(SimpleVertexShader* vs, SimplePixelShader* ps);

	/**
	 * @brief Add an object to the scene
	 * 
	 * @param name The name of the object, default to "GameObject"
	 * @return Object* The pointer of the added object
	 */
	Object* AddObject(std::string name = "GameObject");
	/**
	 * @brief Instantiate a new object from a current object
	 * 
	 * @param obj The template object used to instantiate the new one
	 * @return Object* The pointer of the instantiated object
	 * 
	 * @todo THIS DOES NOTHING! DON'T FORGET TO IMPLEMENT THIS
	 */
	Object* Instantiate(Object* obj);
	/**
	 * @brief Destror an object in the scene graph
	 * 
	 * Nothing happens if there's no such object
	 * 
	 * @param obj The pointer of the object to be deleted
	 * 
	 * @todo THIS DOES NOTHING! DON'T FORGET TO IMPLEMENT THIS
	 */
	void DestroyObject(Object* obj);

	/**
	 * @brief Get the list of all objects
	 * 
	 * @return std::list<Object*> The list of all objects
	 */
	std::list<Object*> GetAllObjects();

	/**
	 * @brief Load a model file and add the object(s) to the scene
	 * 
	 * @param filename The filename of the model
	 * @return Object* The pointer of the parent object added to the scene from the model file
	 */
	Object* LoadModelFile(const std::string& filename);

	/**
	 * @brief Add a light to the scene
	 * 
	 * @param light The data of the light
	 */
	void AddLight(LightData light);
	/**
	 * @brief Get the light
	 * 
	 * @param index The index of the light
	 * @return Light* The pointer of the Light, nullptr if the index is invalid
	 * 
	 * @todo The "nullptr" part is not implemented
	 */
	Light* GetLightAt(int index);
	/**
	 * @brief Get the Light Count object
	 * 
	 * @return int The number of Lights
	 */
	int GetLightCount();
	/**
	 * @brief Remove a light at a specified location
	 * 
	 * Nothing happens if the index is invalid
	 * 
	 * @param index The index of the light to be removed
	 * 
	 * @todo The "invalid" part is not implemented
	 */
	void RemoveLightAt(int index);

	/**
	 * @brief Update the scene
	 * 
	 * Update the camera, the objects and the lights
	 * 
	 * @param deltaTime The time a frame costs
	 * @param totalTime The total time from the beginning of the application
	 */
	void Update(float deltaTime, float totalTime);

	/**
	 * @brief The main camera of the scene
	 * 
	 */
	Camera* mainCamera;
private:
	/**
	 * @brief Recursively add objects to the scene from a node of the model file
	 * 
	 * @param modelFileName The file name of the model
	 * @param scene The imported model file data structure
	 * @param node Currently processing node
	 * @param parent The direct parent object of the model being added
	 * @return Object* The pointer of the object added to the scene
	 */
	Object* AddObjectWithNode(const std::string& modelFileName, const aiScene* scene, aiNode* node, Object* parent);

	/**
	 * @brief All objects
	 * 
	 */
	std::list<Object*> allObjects;
	/**
	 * @brief All light data, will be sent to the shader
	 * 
	 */
	std::vector<LightData> lightData;
	/**
	 * @brief All lights
	 * 
	 */
	std::list<Light*> lights;

	/**
	 * @brief Direct3D 11 device
	 * 
	 */
	ID3D11Device* device;
	/**
	 * @brief Direct3D 11 device context
	 * 
	 */
	ID3D11DeviceContext* context;

	/**
	 * @brief The default vertex shader
	 * 
	 */
	SimpleVertexShader* defaultVS;
	/**
	 * @brief The default pixel shader
	 * 
	 */
	SimplePixelShader* defaultPS;

	/**
	 * @brief The axis-aligned bounding box of the whole scene
	 * 
	 */
	DirectX::BoundingBox aabb;
};

inline Scene::Scene()
{
	mainCamera = new Camera(this);

	device = nullptr;
	context = nullptr;
}

inline Scene::~Scene()
{
	delete mainCamera;
	for (Object* object : allObjects)
	{
		delete object;
	}
	allObjects.clear();
}

inline void Scene::SetD3D11Device(ID3D11Device* d, ID3D11DeviceContext* c)
{
	device = d;

	context = c;
}

inline void Scene::SetDefaultShader(SimpleVertexShader* vs, SimplePixelShader* ps)
{
	defaultVS = vs;
	defaultPS = ps;
}

inline Object* Scene::AddObject(std::string name)
{
	Object* newObj = new Object(this, name);
	allObjects.push_back(newObj);
	return newObj;
}


inline Object* Scene::Instantiate(Object* obj)
{
}

inline void Scene::DestroyObject(Object* obj)
{
	const auto result = std::find(allObjects.begin(), allObjects.end(), obj);
	if (result == allObjects.end()) return;
	allObjects.erase(result);
	delete obj;
}

inline std::list<Object*> Scene::GetAllObjects()
{
	return allObjects;
}

inline Object* Scene::LoadModelFile(const std::string& filename)
{
	// Create an instance of the Importer class
	Assimp::Importer importer;

	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll 
	// propably to request more postprocessing than we do in this example.
	const aiScene* scene = importer.ReadFile(filename,
		aiProcess_CalcTangentSpace
		| aiProcess_Triangulate
		| aiProcess_JoinIdenticalVertices
		| aiProcess_SortByPType
		| aiProcess_ConvertToLeftHanded
		| aiProcess_GenSmoothNormals
		//| aiProcess_PreTransformVertices
	);

	// If the import failed, report it
	if (!scene)
	{
		LOG_ERROR << importer.GetErrorString();
		return nullptr;
	}

	aiNode* currentNode = scene->mRootNode;

	Object* newObj = AddObjectWithNode(filename, scene, currentNode, nullptr);

	importer.FreeScene();

	return newObj;
}

inline void Scene::AddLight(LightData light)
{
	lightData.push_back(light);

	Light* newLight = new Light(&lightData.back(), device, context, mainCamera, &aabb);
	lights.push_back(newLight);
}

inline Light* Scene::GetLightAt(int index)
{
	auto it = lights.begin();
	std::advance(it, index);
	return *it;
}

inline int Scene::GetLightCount()
{
	return int(lightData.size());
}

inline void Scene::RemoveLightAt(int index)
{
	lightData.erase(lightData.begin() + index);

	auto it = lights.begin();
	std::advance(it, index);
	Light* lightToBeRemoved = *it;
	delete lightToBeRemoved;
	lights.erase(it);
}

inline void Scene::Update(float deltaTime, float totalTime)
{
	mainCamera->Update(deltaTime, totalTime);

	const float inf = std::numeric_limits<float>::infinity();
	DirectX::XMVECTOR lower = DirectX::XMVectorSet(inf, inf, inf, inf);
	DirectX::XMVECTOR upper = DirectX::XMVectorSet(-inf, -inf, -inf, -inf);

	for (Object* object : allObjects)
	{
		object->Update(deltaTime, totalTime);

		// Update AABB
		std::list<MeshRenderer*> meshRenderers = object->GetComponents<MeshRenderer>();
		for (MeshRenderer* renderer : meshRenderers)
		{
			//renderer->GetMesh()->aabb;
			DirectX::BoundingBox transformedAABB;
			renderer->GetMesh()->aabb.Transform(transformedAABB, renderer->object->transform->GetGlobalWorldMatrix());

			DirectX::XMVECTOR center = XMLoadFloat3(&transformedAABB.Center);
			DirectX::XMVECTOR extend = XMLoadFloat3(&transformedAABB.Extents);

			DirectX::XMVECTOR minPt = DirectX::XMVectorSubtract(center, extend);
			DirectX::XMVECTOR maxPt = DirectX::XMVectorAdd(center, extend);

			lower = DirectX::XMVectorMin(lower, minPt);
			upper = DirectX::XMVectorMax(upper, maxPt);
		}
	}

	DirectX::BoundingBox::CreateFromPoints(aabb, lower, upper);

	for (Light* light : lights)
	{
		light->UpdateMatrices();
	}
}

inline Object* Scene::AddObjectWithNode(const std::string& modelFileName, const aiScene * scene, aiNode * node, Object * parent)
{
	Object* newObj = AddObject(node->mName.C_Str());

	// Set Parent
	if (parent != nullptr)
		newObj->transform->SetParent(parent->transform);

	// Add MeshRenderer to this object
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		const unsigned int meshIndex = node->mMeshes[i];

		aiMesh* aMesh = scene->mMeshes[meshIndex];

		std::vector<Vertex> vertices;
		std::vector<int> indices;

		vertices.reserve(aMesh->mNumVertices);
		unsigned int indicesCount = 0;
		for (UINT c = 0; c < aMesh->mNumFaces; c++)
			indicesCount += aMesh->mFaces[c].mNumIndices;
		indices.reserve(indicesCount);

		for (UINT j = 0; j < aMesh->mNumVertices; j++)
		{
			Vertex newVtx;
			newVtx.Position = { aMesh->mVertices[j].x, aMesh->mVertices[j].y, aMesh->mVertices[j].z };
			if (aMesh->HasNormals())
				newVtx.Normal = { aMesh->mNormals[j].x, aMesh->mNormals[j].y, aMesh->mNormals[j].z };
			if (aMesh->HasTangentsAndBitangents())
			{
				newVtx.Tangent = { aMesh->mTangents[j].x, aMesh->mTangents[j].y, aMesh->mTangents[j].z };
			}
			if (aMesh->HasTextureCoords(0))
				newVtx.UV = { aMesh->mTextureCoords[0][j].x,aMesh->mTextureCoords[0][j].y };

			vertices.push_back(newVtx);
		}

		for (UINT c = 0; c < aMesh->mNumFaces; c++)
			for (UINT e = 0; e < aMesh->mFaces[c].mNumIndices; e++)
				indices.push_back(int(aMesh->mFaces[c].mIndices[e]));

		// MeshRenderer
		auto * meshRendererComponent = newObj->AddComponent<MeshRenderer>();

		// Material
		std::shared_ptr<PBRMaterial> pbrMaterial = std::make_shared<PBRMaterial>(defaultVS, defaultPS, device, context);

		// Load Textures
		aiMaterial * aMaterial = scene->mMaterials[aMesh->mMaterialIndex];

		// Diffuse Texture
		unsigned int diffuseTextureCount = aMaterial->GetTextureCount(aiTextureType_DIFFUSE);
		if (diffuseTextureCount > 0)
		{
			aiString as;
			aiReturn ar;
			// Only get the first texture
			ar = aMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &as);
			if (ar == aiReturn_SUCCESS)
			{
				// Try load the texture assuming the path is absolute
				if (!pbrMaterial->LoadDiffuseTexture(as.C_Str()))
				{
					// Try the path relative to model file
					boost::filesystem::path modelPath(modelFileName);
					boost::filesystem::path modelFolder = modelPath.parent_path();
					boost::filesystem::path relativeTexture(as.C_Str());
					relativeTexture = modelFolder / relativeTexture;
					pbrMaterial->LoadDiffuseTexture(relativeTexture.generic_string());
				}
			}
		}

		// Normal Texture
		unsigned int normalTextureCount = aMaterial->GetTextureCount(aiTextureType_NORMALS);
		if (normalTextureCount > 0)
		{
			aiString as;
			aiReturn ar;
			// Only get the first texture
			ar = aMaterial->GetTexture(aiTextureType_NORMALS, 0, &as);
			if (ar == aiReturn_SUCCESS)
			{
				// Try load the texture assuming the path is absolute
				if (!pbrMaterial->LoadNormalTexture(as.C_Str()))
				{
					// Try the path relative to model file
					boost::filesystem::path modelPath(modelFileName);
					boost::filesystem::path modelFolder = modelPath.parent_path();
					boost::filesystem::path relativeTexture(as.C_Str());
					relativeTexture = modelFolder / relativeTexture;
					pbrMaterial->LoadNormalTexture(relativeTexture.generic_string());
				}
			}
		}

		meshRendererComponent->SetMaterial(pbrMaterial);

		// Mesh
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(&*vertices.begin(), int(vertices.size()), &*indices.begin(), int(indices.size()), device);
		meshRendererComponent->SetMesh(mesh);
	}

	// Add child nodes
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		Object* childObj = AddObjectWithNode(modelFileName, scene, node->mChildren[i], newObj);
	}

	return newObj;
}


#pragma once
#pragma warning(disable:4251)
#include <d3d11.h>

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

class Scene
{
public:
	friend class DSSRendering;

	Scene();
	~Scene();

	void SetD3D11Device(ID3D11Device* d, ID3D11DeviceContext* c);
	void SetDefaultShader(SimpleVertexShader* vs, SimplePixelShader* ps);

	Object* AddObject(std::string name = "GameObject");
	Object* Instantiate(Object* obj);
	void DestroyObject(Object* obj);

	std::list<Object*> GetAllObjects();

	Object* LoadModelFile(std::string filename);

	void AddLight(Light light);
	Light* GetLightAt(int index);
	int GetLightCount();
	void RemoveLightAt(int index);

	void Update(float deltaTime, float totalTime);

	Camera* mainCamera;
private:
	Object* AddObjectWithNode(std::string modelFileName, const aiScene* scene, aiNode* node, Object* parent);

	std::list<Object*> allObjects;
	std::vector<Light> lights;

	ID3D11Device* device;
	ID3D11DeviceContext* context;

	SimpleVertexShader* defaultVS;
	SimplePixelShader* defaultPS;
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

inline Object* Scene::LoadModelFile(std::string filename)
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
		| aiProcess_PreTransformVertices
	);

	// If the import failed, report it
	if (!scene)
	{
		LOG_ERROR << importer.GetErrorString();
		return nullptr;
	}

	aiNode* currentNode = scene->mRootNode;

	Object* newObj = AddObjectWithNode(filename, scene, currentNode, nullptr);

	return newObj;
}

inline void Scene::AddLight(Light light)
{
	lights.push_back(light);
}

inline Light* Scene::GetLightAt(int index)
{
	return &*(lights.begin() + index);
}

inline int Scene::GetLightCount()
{
	return int(lights.size());
}

inline void Scene::RemoveLightAt(int index)
{
	lights.erase(lights.begin() + index);
}

inline void Scene::Update(float deltaTime, float totalTime)
{
	mainCamera->Update(deltaTime, totalTime);
	for (Object* object : allObjects)
	{
		object->Update(deltaTime, totalTime);
	}
}

inline Object* Scene::AddObjectWithNode(std::string modelFileName, const aiScene * scene, aiNode * node, Object * parent)
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
			newVtx.Normal = { aMesh->mNormals[j].x, aMesh->mNormals[j].y, aMesh->mNormals[j].z };
			newVtx.Tangent = { aMesh->mTangents[j].x, aMesh->mTangents[j].y, aMesh->mTangents[j].z };
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
		aiMaterial* aMaterial = scene->mMaterials[aMesh->mMaterialIndex];

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


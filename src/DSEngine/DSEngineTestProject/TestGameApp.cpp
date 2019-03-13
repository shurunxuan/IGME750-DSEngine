#include <iostream>

#include <assimp/Importer.hpp> 
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include "TestGameApp.h"
#include "PressSpaceToPlayAudio.h"
#include "UnlitMaterial.h"
#include "MoveParentObject.h"
#include "CameraController.h"
#include "PBRMaterial.h"

TestGameApp::~TestGameApp()
{
	delete vertexShader;
	delete pbrPixelShader;
}

void TestGameApp::Init()
{
	// Stub logic
	LOG_TRACE << "TestGameApp Init";

	// Load Shaders
	// TODO: Could this be done by a resource manager?
	vertexShader = new SimpleVertexShader(device, context);
	pbrPixelShader = new SimplePixelShader(device, context);

	vertexShader->LoadShaderFile(L"VertexShader.cso");
	pbrPixelShader->LoadShaderFile(L"PBRPixelShader.cso");

	// Set Camera
	CurrentActiveScene()->mainCamera->UpdateProjectionMatrix(float(width), float(height), 3.1415926f / 4.0f);
	CurrentActiveScene()->mainCamera->SetSkybox(device, context, L"Assets/Skybox/1/Environment1HiDef.cubemap.dds", L"Assets/Skybox/1/Environment1Light.cubemap.dds");
	//CurrentActiveScene()->mainCamera->SetSkybox(device, context, L"Assets/Skybox/mp_cupertin/mp_cupertin.dds", L"Assets/Skybox/mp_cupertin/mp_cupertin_irr.dds");
	CameraController * cameraController = CurrentActiveScene()->mainCamera->AddComponent<CameraController>();

	// Add a light
	Light light = DirectionalLight(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), 1.0f, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));;
	CurrentActiveScene()->AddLight(light);

	// Add parent object
	Object * parentObj = CurrentActiveScene()->AddObject("NewObject");
	parentObj->transform->SetLocalScale(0.05f, 0.05f, 0.05f);
	parentObj->transform->SetLocalTranslation(0.0f, 0.0f, 5.0f);


	// Create an instance of the Importer class
	Assimp::Importer importer;

	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll 
	// propably to request more postprocessing than we do in this example.
	const aiScene * scene = importer.ReadFile("Assets/Models/025_Pikachu/0.obj",
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
		return;
	}

	int meshes = scene->mNumMeshes;

	for (UINT i = 0; i < scene->mNumMeshes; i++)
	{
		std::vector<Vertex> vertices;
		std::vector<int> indices;

		aiMesh* aMesh = scene->mMeshes[i];
		
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
		MeshRenderer* meshRendererComponent = parentObj->AddComponent<MeshRenderer>();
		// Material
		std::shared_ptr<PBRMaterial> pbrMaterial = std::make_shared<PBRMaterial>(vertexShader, pbrPixelShader, device, context);
		// Yellow
		pbrMaterial->parameters.albedo = { 1.0f, 0.765557f, 0.336057f };
		meshRendererComponent->SetMaterial(pbrMaterial);
		// Mesh

		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(&*vertices.begin(), int(vertices.size()), &*indices.begin(), int(indices.size()), device);
		meshRendererComponent->SetMesh(mesh);
	}

	// Add Components
	PressSpaceToPlayAudio* playAudioComponent = parentObj->AddComponent<PressSpaceToPlayAudio>();
	MoveParentObject* moveParentComponent = parentObj->AddComponent<MoveParentObject>();


}

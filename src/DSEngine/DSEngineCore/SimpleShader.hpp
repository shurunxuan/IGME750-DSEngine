#pragma once
#pragma warning(disable:4251)
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <unordered_map>
#include <vector>
#include <string>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

// --------------------------------------------------------
// Used by simple shaders to store information about
// specific variables in constant buffers
// --------------------------------------------------------
struct SimpleShaderVariable
{
	unsigned int ByteOffset;
	unsigned int Size;
	unsigned int ConstantBufferIndex;
};

// --------------------------------------------------------
// Contains information about a specific
// constant buffer in a shader, as well as
// the local data buffer for it
// --------------------------------------------------------
struct SimpleConstantBuffer
{
	std::string Name;
	D3D_CBUFFER_TYPE Type;
	unsigned int Size;
	unsigned int BindIndex;
	ID3D11Buffer* ConstantBuffer;
	unsigned char* LocalDataBuffer;
	std::vector<SimpleShaderVariable> Variables;
};

// --------------------------------------------------------
// Contains info about a single SRV in a shader
// --------------------------------------------------------
struct SimpleSRV
{
	unsigned int Index;		// The raw index of the SRV
	unsigned int BindIndex; // The register of the SRV
};

// --------------------------------------------------------
// Contains info about a single Sampler in a shader
// --------------------------------------------------------
struct SimpleSampler
{
	unsigned int Index;		// The raw index of the Sampler
	unsigned int BindIndex; // The register of the Sampler
};

// --------------------------------------------------------
// Base abstract class for simplifying shader handling
// --------------------------------------------------------
class ISimpleShader
{
public:
	ISimpleShader(ID3D11Device* device, ID3D11DeviceContext* context);
	virtual ~ISimpleShader();

	// Initialization method (since we can't invoke derived class
	// overrides in the base class constructor)
	bool LoadShaderFile(LPCWSTR shaderFile);

	// Simple helpers
	bool IsShaderValid() { return shaderValid; }

	// Activating the shader and copying data
	void SetShader();
	void CopyAllBufferData();
	void CopyBufferData(unsigned int index);
	void CopyBufferData(std::string bufferName);

	// Sets arbitrary shader data
	bool SetData(std::string name, const void* data, unsigned int size);

	bool SetInt(std::string name, int data);
	bool SetFloat(std::string name, float data);
	bool SetFloat2(std::string name, const float data[2]);
	bool SetFloat2(std::string name, const DirectX::XMFLOAT2 data);
	bool SetFloat3(std::string name, const float data[3]);
	bool SetFloat3(std::string name, const DirectX::XMFLOAT3 data);
	bool SetFloat4(std::string name, const float data[4]);
	bool SetFloat4(std::string name, const DirectX::XMFLOAT4 data);
	bool SetMatrix4x4(std::string name, const float data[16]);
	bool SetMatrix4x4(std::string name, const DirectX::XMFLOAT4X4 data);

	// Setting shader resources
	virtual bool SetShaderResourceView(std::string name, ID3D11ShaderResourceView* srv) = 0;
	virtual bool SetSamplerState(std::string name, ID3D11SamplerState* samplerState) = 0;

	// Getting data about variables and resources
	const SimpleShaderVariable* GetVariableInfo(std::string name);
	
	const SimpleSRV* GetShaderResourceViewInfo(std::string name);
	const SimpleSRV* GetShaderResourceViewInfo(unsigned int index);
	size_t GetShaderResourceViewCount() { return textureTable.size(); }
	
	const SimpleSampler* GetSamplerInfo(std::string name);
	const SimpleSampler* GetSamplerInfo(unsigned int index);
	size_t GetSamplerCount() { return samplerTable.size(); }

	// Get data about constant buffers
	unsigned int GetBufferCount();
	unsigned int GetBufferSize(unsigned int index);
	const SimpleConstantBuffer* GetBufferInfo(std::string name);
	const SimpleConstantBuffer* GetBufferInfo(unsigned int index);
	
	// Misc getters
	ID3DBlob* GetShaderBlob() { return shaderBlob; }

protected:
	
	bool shaderValid;
	ID3DBlob* shaderBlob;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	// Resource counts
	unsigned int constantBufferCount;
	
	// Maps for variables and buffers
	SimpleConstantBuffer*		constantBuffers; // For index-based lookup
	std::vector<SimpleSRV*>		shaderResourceViews;
	std::vector<SimpleSampler*>	samplerStates;
	std::unordered_map<std::string, SimpleConstantBuffer*> cbTable;
	std::unordered_map<std::string, SimpleShaderVariable> varTable;
	std::unordered_map<std::string, SimpleSRV*> textureTable;
	std::unordered_map<std::string, SimpleSampler*> samplerTable;

	// Pure virtual functions for dealing with shader types
	virtual bool CreateShader(ID3DBlob* shaderBlob) = 0;
	virtual void SetShaderAndCBs() = 0;

	virtual void CleanUp();

	// Helpers for finding data by name
	SimpleShaderVariable* FindVariable(std::string name, int size);
	SimpleConstantBuffer* FindConstantBuffer(std::string name);
};

// --------------------------------------------------------
// Derived class for VERTEX shaders ///////////////////////
// --------------------------------------------------------
class SimpleVertexShader : public ISimpleShader
{
public:
	SimpleVertexShader(ID3D11Device* device, ID3D11DeviceContext* context);
	SimpleVertexShader(ID3D11Device* device, ID3D11DeviceContext* context, ID3D11InputLayout* inputLayout, bool perInstanceCompatible);
	~SimpleVertexShader();
	ID3D11VertexShader* GetDirectXShader() { return shader; }
	ID3D11InputLayout* GetInputLayout() { return inputLayout; }
	bool GetPerInstanceCompatible() { return perInstanceCompatible; }

	bool SetShaderResourceView(std::string name, ID3D11ShaderResourceView* srv);
	bool SetSamplerState(std::string name, ID3D11SamplerState* samplerState);

protected:
	bool perInstanceCompatible;
	ID3D11InputLayout* inputLayout;
	ID3D11VertexShader* shader;
	bool CreateShader(ID3DBlob* shaderBlob);
	void SetShaderAndCBs();
	void CleanUp();
};


// --------------------------------------------------------
// Derived class for PIXEL shaders ////////////////////////
// --------------------------------------------------------
class SimplePixelShader : public ISimpleShader
{
public:
	SimplePixelShader(ID3D11Device* device, ID3D11DeviceContext* context);
	~SimplePixelShader();
	ID3D11PixelShader* GetDirectXShader() { return shader; }

	bool SetShaderResourceView(std::string name, ID3D11ShaderResourceView* srv);
	bool SetSamplerState(std::string name, ID3D11SamplerState* samplerState);

protected:
	ID3D11PixelShader* shader;
	bool CreateShader(ID3DBlob* shaderBlob);
	void SetShaderAndCBs();
	void CleanUp();
};

// --------------------------------------------------------
// Derived class for DOMAIN shaders ///////////////////////
// --------------------------------------------------------
class SimpleDomainShader : public ISimpleShader
{
public:
	SimpleDomainShader(ID3D11Device* device, ID3D11DeviceContext* context);
	~SimpleDomainShader();
	ID3D11DomainShader* GetDirectXShader() { return shader; }

	bool SetShaderResourceView(std::string name, ID3D11ShaderResourceView* srv);
	bool SetSamplerState(std::string name, ID3D11SamplerState* samplerState);

protected:
	ID3D11DomainShader* shader;
	bool CreateShader(ID3DBlob* shaderBlob);
	void SetShaderAndCBs();
	void CleanUp();
};

// --------------------------------------------------------
// Derived class for HULL shaders /////////////////////////
// --------------------------------------------------------
class SimpleHullShader : public ISimpleShader
{
public:
	SimpleHullShader(ID3D11Device* device, ID3D11DeviceContext* context);
	~SimpleHullShader();
	ID3D11HullShader* GetDirectXShader() { return shader; }

	bool SetShaderResourceView(std::string name, ID3D11ShaderResourceView* srv);
	bool SetSamplerState(std::string name, ID3D11SamplerState* samplerState);

protected:
	ID3D11HullShader* shader;
	bool CreateShader(ID3DBlob* shaderBlob);
	void SetShaderAndCBs();
	void CleanUp();
};

// --------------------------------------------------------
// Derived class for GEOMETRY shaders /////////////////////
// --------------------------------------------------------
class SimpleGeometryShader : public ISimpleShader
{
public:
	SimpleGeometryShader(ID3D11Device* device, ID3D11DeviceContext* context, bool useStreamOut = 0, bool allowStreamOutRasterization = 0);
	~SimpleGeometryShader();
	ID3D11GeometryShader* GetDirectXShader() { return shader; }

	bool SetShaderResourceView(std::string name, ID3D11ShaderResourceView* srv);
	bool SetSamplerState(std::string name, ID3D11SamplerState* samplerState);

	bool CreateCompatibleStreamOutBuffer(ID3D11Buffer** buffer, int vertexCount);

	static void UnbindStreamOutStage(ID3D11DeviceContext* deviceContext);

protected:
	// Shader itself
	ID3D11GeometryShader* shader;

	// Stream out related
	bool useStreamOut;
	bool allowStreamOutRasterization;
	unsigned int streamOutVertexSize;

	bool CreateShader(ID3DBlob* shaderBlob);
	bool CreateShaderWithStreamOut(ID3DBlob* shaderBlob);
	void SetShaderAndCBs();
	void CleanUp();

	// Helpers
	unsigned int CalcComponentCount(unsigned int mask);
};


// --------------------------------------------------------
// Derived class for COMPUTE shaders //////////////////////
// --------------------------------------------------------
class SimpleComputeShader : public ISimpleShader
{
public:
	SimpleComputeShader(ID3D11Device* device, ID3D11DeviceContext* context);
	~SimpleComputeShader();
	ID3D11ComputeShader* GetDirectXShader() { return shader; }

	void DispatchByGroups(unsigned int groupsX, unsigned int groupsY, unsigned int groupsZ);
	void DispatchByThreads(unsigned int threadsX, unsigned int threadsY, unsigned int threadsZ);

	bool SetShaderResourceView(std::string name, ID3D11ShaderResourceView* srv);
	bool SetSamplerState(std::string name, ID3D11SamplerState* samplerState);
	bool SetUnorderedAccessView(std::string name, ID3D11UnorderedAccessView* uav, unsigned int appendConsumeOffset = -1);

	int GetUnorderedAccessViewIndex(std::string name);

protected:
	ID3D11ComputeShader* shader;
	std::unordered_map<std::string, unsigned int> uavTable;

	unsigned int threadsX;
	unsigned int threadsY;
	unsigned int threadsZ;
	unsigned int threadsTotal;

	bool CreateShader(ID3DBlob* shaderBlob);
	void SetShaderAndCBs();
	void CleanUp();
};

///////////////////////////////////////////////////////////////////////////////
// ------ BASE SIMPLE SHADER --------------------------------------------------
///////////////////////////////////////////////////////////////////////////////

// --------------------------------------------------------
// Constructor accepts DirectX device & context
// --------------------------------------------------------
inline ISimpleShader::ISimpleShader(ID3D11Device* device, ID3D11DeviceContext* context)
{
	// Save the device
	this->device = device;
	this->deviceContext = context;

	// Set up fields
	constantBufferCount = 0;
	constantBuffers = 0;
	shaderBlob = 0;
}

// --------------------------------------------------------
// Destructor
// --------------------------------------------------------
inline ISimpleShader::~ISimpleShader()
{
	// Derived class destructors will call this class's CleanUp method
	if(shaderBlob)
		shaderBlob->Release();
}

// --------------------------------------------------------
// Cleans up the variable table and buffers - Some things will
// be handled by derived classes
// --------------------------------------------------------
inline void ISimpleShader::CleanUp()
{
	// Handle constant buffers and local data buffers
	for (unsigned int i = 0; i < constantBufferCount; i++)
	{
		constantBuffers[i].ConstantBuffer->Release();
		delete[] constantBuffers[i].LocalDataBuffer;
	}

	if (constantBuffers)
	{
		delete[] constantBuffers;
		constantBufferCount = 0;
	}

	for (unsigned int i = 0; i < shaderResourceViews.size(); i++)
		delete shaderResourceViews[i];
	
	for (unsigned int i = 0; i < samplerStates.size(); i++)
		delete samplerStates[i];

	// Clean up tables
	varTable.clear();
	cbTable.clear();
	samplerTable.clear();
	textureTable.clear();
}

// --------------------------------------------------------
// Loads the specified shader and builds the variable table using shader
// reflection.  This must be a separate step from the constructor since
// we can't invoke derived class overrides in the base class constructor.
//
// shaderFile - A "wide string" specifying the compiled shader to load
// 
// Returns true if shader is loaded properly, false otherwise
// --------------------------------------------------------
inline bool ISimpleShader::LoadShaderFile(LPCWSTR shaderFile)
{
	// Load the shader to a blob and ensure it worked
	HRESULT hr = D3DReadFileToBlob(shaderFile, &shaderBlob);
	if (hr != S_OK)
	{
		return false;
	}

	// Create the shader - Calls an overloaded version of this abstract
	// method in the appropriate child class
	shaderValid = CreateShader(shaderBlob);
	if (!shaderValid)
	{
		return false;
	}

	// Set up shader reflection to get information about
	// this shader and its variables,  buffers, etc.
	ID3D11ShaderReflection* refl;
	D3DReflect(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		IID_ID3D11ShaderReflection,
		(void**)&refl);
	
	// Get the description of the shader
	D3D11_SHADER_DESC shaderDesc;
	refl->GetDesc(&shaderDesc);

	// Get the number of buffers and make the resource array
	constantBufferCount = shaderDesc.ConstantBuffers;
	constantBuffers = new SimpleConstantBuffer[constantBufferCount];
	
	// Handle bound resources (like shaders and samplers)
	unsigned int resourceCount = shaderDesc.BoundResources;
	for (unsigned int r = 0; r < resourceCount; r++)
	{
		// Get this resource's description
		D3D11_SHADER_INPUT_BIND_DESC resourceDesc;
		refl->GetResourceBindingDesc(r, &resourceDesc);

		// Check the type
		switch (resourceDesc.Type)
		{
		case D3D_SIT_TEXTURE: // A texture resource
		{
			// Create the SRV wrapper
			SimpleSRV* srv = new SimpleSRV();
			srv->BindIndex = resourceDesc.BindPoint;				// Shader bind point
			srv->Index = (unsigned int)shaderResourceViews.size();	// Raw index

			textureTable.insert(std::pair<std::string, SimpleSRV*>(resourceDesc.Name, srv));
			shaderResourceViews.push_back(srv);
		}
			break;

		case D3D_SIT_SAMPLER: // A sampler resource
		{
			// Create the sampler wrapper
			SimpleSampler* samp = new SimpleSampler();
			samp->BindIndex = resourceDesc.BindPoint;			// Shader bind point
			samp->Index = (unsigned int)samplerStates.size();	// Raw index

			samplerTable.insert(std::pair<std::string, SimpleSampler*>(resourceDesc.Name, samp));
			samplerStates.push_back(samp);
		}
			break;
		}
	}

	// Loop through all constant buffers
	for (unsigned int b = 0; b < constantBufferCount; b++)
	{
		// Get this buffer
		ID3D11ShaderReflectionConstantBuffer* cb =
			refl->GetConstantBufferByIndex(b);
		
		// Get the description of this buffer
		D3D11_SHADER_BUFFER_DESC bufferDesc;
		cb->GetDesc(&bufferDesc);
		
		// Save the type, which we reference when setting these buffers
		constantBuffers[b].Type = bufferDesc.Type;

		// Get the description of the resource binding, so
		// we know exactly how it's bound in the shader
		D3D11_SHADER_INPUT_BIND_DESC bindDesc;
		refl->GetResourceBindingDescByName(bufferDesc.Name, &bindDesc);
		
		// Set up the buffer and put its pointer in the table
		constantBuffers[b].BindIndex = bindDesc.BindPoint;
		constantBuffers[b].Name = bufferDesc.Name;
		cbTable.insert(std::pair<std::string, SimpleConstantBuffer*>(bufferDesc.Name, &constantBuffers[b]));

		// Create this constant buffer
		D3D11_BUFFER_DESC newBuffDesc;
		newBuffDesc.Usage = D3D11_USAGE_DEFAULT;
		newBuffDesc.ByteWidth = max(bufferDesc.Size, 16); // NEW: Must be multiple of 16
		newBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		newBuffDesc.CPUAccessFlags = 0;
		newBuffDesc.MiscFlags = 0;
		newBuffDesc.StructureByteStride = 0;
		device->CreateBuffer(&newBuffDesc, 0, &constantBuffers[b].ConstantBuffer);

		// Set up the data buffer for this constant buffer
		constantBuffers[b].Size = bufferDesc.Size;
		constantBuffers[b].LocalDataBuffer = new unsigned char[bufferDesc.Size];
		ZeroMemory(constantBuffers[b].LocalDataBuffer, bufferDesc.Size);

		// Loop through all variables in this buffer
		for (unsigned int v = 0; v < bufferDesc.Variables; v++)
		{
			// Get this variable
			ID3D11ShaderReflectionVariable* var =
				cb->GetVariableByIndex(v);
			
			// Get the description of the variable and its type
			D3D11_SHADER_VARIABLE_DESC varDesc;
			var->GetDesc(&varDesc);

			// Create the variable struct
			SimpleShaderVariable varStruct;
			varStruct.ConstantBufferIndex = b;
			varStruct.ByteOffset = varDesc.StartOffset;
			varStruct.Size = varDesc.Size;
			
			// Get a string version
			std::string varName(varDesc.Name);

			// Add this variable to the table and the constant buffer
			varTable.insert(std::pair<std::string, SimpleShaderVariable>(varName, varStruct));
			constantBuffers[b].Variables.push_back(varStruct);
		}
	}

	// All set
	refl->Release();
	return true;
}

// --------------------------------------------------------
// Helper for looking up a variable by name and also
// verifying that it is the requested size
// 
// name - the name of the variable to look for
// size - the size of the variable (for verification), or -1 to bypass
// --------------------------------------------------------
inline SimpleShaderVariable* ISimpleShader::FindVariable(std::string name, int size)
{
	// Look for the key
	std::unordered_map<std::string, SimpleShaderVariable>::iterator result =
		varTable.find(name);

	// Did we find the key?
	if (result == varTable.end())
		return 0;

	// Grab the result from the iterator
	SimpleShaderVariable* var = &(result->second);

	// Is the data size correct ?
	if (size > 0 && var->Size != size)
		return 0;

	// Success
	return var;
}

// --------------------------------------------------------
// Helper for looking up a constant buffer by name
// --------------------------------------------------------
inline SimpleConstantBuffer* ISimpleShader::FindConstantBuffer(std::string name)
{
	// Look for the key
	std::unordered_map<std::string, SimpleConstantBuffer*>::iterator result =
		cbTable.find(name);

	// Did we find the key?
	if (result == cbTable.end())
		return 0;

	// Success
	return result->second;
}

// --------------------------------------------------------
// Sets the shader and associated constant buffers in DirectX
// --------------------------------------------------------
inline void ISimpleShader::SetShader()
{
	// Ensure the shader is valid
	if (!shaderValid) return;

	// Set the shader and any relevant constant buffers, which
	// is an overloaded method in a subclass
	SetShaderAndCBs();
}

// --------------------------------------------------------
// Copies the relevant data to the all of this 
// shader's constant buffers.  To just copy one
// buffer, use CopyBufferData()
// --------------------------------------------------------
inline void ISimpleShader::CopyAllBufferData()
{
	// Ensure the shader is valid
	if (!shaderValid) return;

	// Loop through the constant buffers and copy all data
	for (unsigned int i = 0; i < constantBufferCount; i++)
	{
		// Copy the entire local data buffer
		deviceContext->UpdateSubresource(
			constantBuffers[i].ConstantBuffer, 0, 0,
			constantBuffers[i].LocalDataBuffer, 0, 0);
	}
}

// --------------------------------------------------------
// Copies local data to the shader's specified constant buffer
//
// index - The index of the buffer to copy.
//         Useful for updating more frequently-changing
//         variables without having to re-copy all buffers.
//  
// NOTE: The "index" of the buffer might NOT be the same
//       as its register, especially if you have buffers
//       bound to non-sequential registers!
// --------------------------------------------------------
inline void ISimpleShader::CopyBufferData(unsigned int index)
{
	// Ensure the shader is valid
	if (!shaderValid) return;

	// Validate the index
	if(index >= this->constantBufferCount)
		return;

	// Check for the buffer
	SimpleConstantBuffer* cb = &this->constantBuffers[index];
	if (!cb) return;

	// Copy the data and get out
	deviceContext->UpdateSubresource(
		cb->ConstantBuffer, 0, 0, 
		cb->LocalDataBuffer, 0, 0);
}

// --------------------------------------------------------
// Copies local data to the shader's specified constant buffer
//
// bufferName - Specifies the name of the buffer to copy.
//              Useful for updating more frequently-changing
//              variables without having to re-copy all buffers.
// --------------------------------------------------------
inline void ISimpleShader::CopyBufferData(std::string bufferName)
{
	// Ensure the shader is valid
	if (!shaderValid) return;

	// Check for the buffer
	SimpleConstantBuffer* cb = this->FindConstantBuffer(bufferName);
	if (!cb) return;

	// Copy the data and get out
	deviceContext->UpdateSubresource(
		cb->ConstantBuffer, 0, 0, 
		cb->LocalDataBuffer, 0, 0);
}


// --------------------------------------------------------
// Sets a variable by name with arbitrary data of the specified size
//
// name - The name of the shader variable
// data - The data to set in the buffer
// size - The size of the data (this must match the variable's size)
//
// Returns true if data is copied, false if variable doesn't 
// exist or sizes don't match
// --------------------------------------------------------
inline bool ISimpleShader::SetData(std::string name, const void* data, unsigned int size)
{
	// Look for the variable and verify
	SimpleShaderVariable* var = FindVariable(name, size);
	if (var == 0)
		return false;

	// Set the data in the local data buffer
	memcpy(
		constantBuffers[var->ConstantBufferIndex].LocalDataBuffer + var->ByteOffset,
		data,
		size);

	// Success
	return true;
}

// --------------------------------------------------------
// Sets INTEGER data
// --------------------------------------------------------
inline bool ISimpleShader::SetInt(std::string name, int data)
{
	return this->SetData(name, (void*)(&data), sizeof(int));
}

// --------------------------------------------------------
// Sets a FLOAT variable by name in the local data buffer
// --------------------------------------------------------
inline bool ISimpleShader::SetFloat(std::string name, float data)
{
	return this->SetData(name, (void*)(&data), sizeof(float));
}

// --------------------------------------------------------
// Sets a FLOAT2 variable by name in the local data buffer
// --------------------------------------------------------
inline bool ISimpleShader::SetFloat2(std::string name, const float data[2])
{
	return this->SetData(name, (void*)data, sizeof(float) * 2);
}

// --------------------------------------------------------
// Sets a FLOAT2 variable by name in the local data buffer
// --------------------------------------------------------
inline bool ISimpleShader::SetFloat2(std::string name, const DirectX::XMFLOAT2 data)
{
	return this->SetData(name, &data, sizeof(float) * 2);
}

// --------------------------------------------------------
// Sets a FLOAT3 variable by name in the local data buffer
// --------------------------------------------------------
inline bool ISimpleShader::SetFloat3(std::string name, const float data[3])
{
	return this->SetData(name, (void*)data, sizeof(float) * 3);
}

// --------------------------------------------------------
// Sets a FLOAT3 variable by name in the local data buffer
// --------------------------------------------------------
inline bool ISimpleShader::SetFloat3(std::string name, const DirectX::XMFLOAT3 data)
{
	return this->SetData(name, &data, sizeof(float) * 3);
}

// --------------------------------------------------------
// Sets a FLOAT4 variable by name in the local data buffer
// --------------------------------------------------------
inline bool ISimpleShader::SetFloat4(std::string name, const float data[4])
{
	return this->SetData(name, (void*)data, sizeof(float) * 4);
}

// --------------------------------------------------------
// Sets a FLOAT4 variable by name in the local data buffer
// --------------------------------------------------------
inline bool ISimpleShader::SetFloat4(std::string name, const DirectX::XMFLOAT4 data)
{
	return this->SetData(name, &data, sizeof(float) * 4);
}

// --------------------------------------------------------
// Sets a MATRIX (4x4) variable by name in the local data buffer
// --------------------------------------------------------
inline bool ISimpleShader::SetMatrix4x4(std::string name, const float data[16])
{
	return this->SetData(name, (void*)data, sizeof(float) * 16);
}

// --------------------------------------------------------
// Sets a MATRIX (4x4) variable by name in the local data buffer
// --------------------------------------------------------
inline bool ISimpleShader::SetMatrix4x4(std::string name, const DirectX::XMFLOAT4X4 data)
{
	return this->SetData(name, &data, sizeof(float) * 16);
}

// --------------------------------------------------------
// Gets info about a shader variable, if it exists
// --------------------------------------------------------
inline const SimpleShaderVariable* ISimpleShader::GetVariableInfo(std::string name)
{
	return FindVariable(name, -1);
}

// --------------------------------------------------------
// Gets info about an SRV in the shader (or null)
//
// name - the name of the SRV
// --------------------------------------------------------
inline const SimpleSRV* ISimpleShader::GetShaderResourceViewInfo(std::string name)
{
	// Look for the key
	std::unordered_map<std::string, SimpleSRV*>::iterator result =
		textureTable.find(name);

	// Did we find the key?
	if (result == textureTable.end())
		return 0;

	// Success
	return result->second;
}


// --------------------------------------------------------
// Gets info about an SRV in the shader (or null)
//
// index - the index of the SRV
// --------------------------------------------------------
inline const SimpleSRV* ISimpleShader::GetShaderResourceViewInfo(unsigned int index)
{
	// Valid index?
	if (index >= shaderResourceViews.size()) return 0;

	// Grab the bind index
	return shaderResourceViews[index];
}


// --------------------------------------------------------
// Gets info about a sampler in the shader (or null)
// 
// name - the name of the sampler
// --------------------------------------------------------
inline const SimpleSampler* ISimpleShader::GetSamplerInfo(std::string name)
{
	// Look for the key
	std::unordered_map<std::string, SimpleSampler*>::iterator result =
		samplerTable.find(name);

	// Did we find the key?
	if (result == samplerTable.end())
		return 0;

	// Success
	return result->second;
}

// --------------------------------------------------------
// Gets info about a sampler in the shader (or null)
// 
// index - the index of the sampler
// --------------------------------------------------------
inline const SimpleSampler* ISimpleShader::GetSamplerInfo(unsigned int index)
{
	// Valid index?
	if (index >= samplerStates.size()) return 0;

	// Grab the bind index
	return samplerStates[index];
}


// --------------------------------------------------------
// Gets the number of constant buffers in this shader
// --------------------------------------------------------
inline unsigned int ISimpleShader::GetBufferCount() { return constantBufferCount; }



// --------------------------------------------------------
// Gets the size of a particular constant buffer, or -1
// --------------------------------------------------------
inline unsigned int ISimpleShader::GetBufferSize(unsigned int index)
{
	// Valid index?
	if (index >= constantBufferCount)
		return -1;

	// Grab the size
	return constantBuffers[index].Size;
}

// --------------------------------------------------------
// Gets info about a particular constant buffer 
// by name, if it exists
// --------------------------------------------------------
inline const SimpleConstantBuffer * ISimpleShader::GetBufferInfo(std::string name)
{
	return FindConstantBuffer(name);
}

// --------------------------------------------------------
// Gets info about a particular constant buffer 
//
// index - the index of the constant buffer
// --------------------------------------------------------
inline const SimpleConstantBuffer * ISimpleShader::GetBufferInfo(unsigned int index)
{
	// Check for valid index
	if (index >= constantBufferCount) return 0;

	// Return the specific buffer
	return &constantBuffers[index];
}





///////////////////////////////////////////////////////////////////////////////
// ------ SIMPLE VERTEX SHADER ------------------------------------------------
///////////////////////////////////////////////////////////////////////////////

// --------------------------------------------------------
// Constructor just calls the base
// --------------------------------------------------------
inline SimpleVertexShader::SimpleVertexShader(ID3D11Device* device, ID3D11DeviceContext* context)
	: ISimpleShader(device, context) 
{ 
	// Ensure we set to zero to successfully trigger
	// the Input Layout creation during LoadShader()
	this->inputLayout = 0;
	this->shader = 0;
	this->perInstanceCompatible = false;
}

// --------------------------------------------------------
// Constructor overload which takes a custom input layout
//
// Passing in a valid input layout will stop LoadShader()
// from creating an input layout from shader reflection
// --------------------------------------------------------
inline SimpleVertexShader::SimpleVertexShader(ID3D11Device * device, ID3D11DeviceContext * context, ID3D11InputLayout * inputLayout, bool perInstanceCompatible)
	: ISimpleShader(device, context)
{
	// Save the custom input layout
	this->inputLayout = inputLayout;
	this->shader = 0;

	// Unable to determine from an input layout, require user to tell us
	this->perInstanceCompatible = perInstanceCompatible;
}

// --------------------------------------------------------
// Destructor - Clean up actual shader (base will be called automatically)
// --------------------------------------------------------
inline SimpleVertexShader::~SimpleVertexShader()
{
	CleanUp();
}

// --------------------------------------------------------
// Handles cleaning up shader and base class clean up
// --------------------------------------------------------
inline void SimpleVertexShader::CleanUp()
{
	ISimpleShader::CleanUp();
	if (shader) { shader->Release(); shader = 0; }
	if (inputLayout) { inputLayout->Release(); inputLayout = 0; }
}

// --------------------------------------------------------
// Creates the DirectX vertex shader
//
// shaderBlob - The shader's compiled code
//
// Returns true if shader is created correctly, false otherwise
// --------------------------------------------------------
inline bool SimpleVertexShader::CreateShader(ID3DBlob* shaderBlob)
{
	// Clean up first, in the event this method is
	// called more than once on the same object
	this->CleanUp();

	// Create the shader from the blob
	HRESULT result = device->CreateVertexShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		0,
		&shader);

	// Did the creation work?
	if (result != S_OK)
		return false;

	// Do we already have an input layout?
	// (This would come from one of the constructor overloads)
	if (inputLayout)
		return true;

	// Vertex shader was created successfully, so we now use the
	// shader code to re-reflect and create an input layout that 
	// matches what the vertex shader expects.  Code adapted from:
	// https://takinginitiative.wordpress.com/2011/12/11/directx-1011-basic-shader-reflection-automatic-input-layout-creation/

	// Reflect shader info
	ID3D11ShaderReflection* refl;
	D3DReflect(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		IID_ID3D11ShaderReflection,
		(void**)&refl);
	
	// Get shader info
	D3D11_SHADER_DESC shaderDesc;
	refl->GetDesc(&shaderDesc);

	// Read input layout description from shader info
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	for (unsigned int i = 0; i< shaderDesc.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		refl->GetInputParameterDesc(i, &paramDesc);

		// Check the semantic name for "_PER_INSTANCE"
		std::string perInstanceStr = "_PER_INSTANCE";
		std::string sem = paramDesc.SemanticName;
		int lenDiff = (int)sem.size() - (int)perInstanceStr.size();
		bool isPerInstance = 
			lenDiff >= 0 &&
			sem.compare(lenDiff, perInstanceStr.size(), perInstanceStr) == 0;

		// Fill out input element desc
		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		// Replace anything affected by "per instance" data
		if (isPerInstance)
		{
			elementDesc.InputSlot = 1; // Assume per instance data comes from another input slot!
			elementDesc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
			elementDesc.InstanceDataStepRate = 1;

			perInstanceCompatible = true;
		}

		// Determine DXGI format
		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		// Save element desc
		inputLayoutDesc.push_back(elementDesc);
	}

	// Try to create Input Layout
	HRESULT hr = device->CreateInputLayout(
		&inputLayoutDesc[0], 
		(unsigned int)inputLayoutDesc.size(), 
		shaderBlob->GetBufferPointer(), 
		shaderBlob->GetBufferSize(),
		&inputLayout);

	// All done, clean up
	refl->Release();
	return true;
}

// --------------------------------------------------------
// Sets the vertex shader, input layout and constant buffers
// for future DirectX drawing
// --------------------------------------------------------
inline void SimpleVertexShader::SetShaderAndCBs()
{
	// Is shader valid?
	if (!shaderValid) return;

	// Set the shader and input layout
	deviceContext->IASetInputLayout(inputLayout);
	deviceContext->VSSetShader(shader, 0, 0);

	// Set the constant buffers
	for (unsigned int i = 0; i < constantBufferCount; i++)
	{
		// Skip "buffers" that aren't true constant buffers
		if (constantBuffers[i].Type != D3D11_CT_CBUFFER)
			continue;

		// This is a real constant buffer, so set it
		deviceContext->VSSetConstantBuffers(
			constantBuffers[i].BindIndex,
			1,
			&constantBuffers[i].ConstantBuffer);
	}
}

// --------------------------------------------------------
// Sets a shader resource view in the vertex shader stage
//
// name - The name of the texture resource in the shader
// srv - The shader resource view of the texture in GPU memory
//
// Returns true if a texture of the given name was found, false otherwise
// --------------------------------------------------------
inline bool SimpleVertexShader::SetShaderResourceView(std::string name, ID3D11ShaderResourceView* srv)
{
	// Look for the variable and verify
	const SimpleSRV* srvInfo = GetShaderResourceViewInfo(name);
	if (srvInfo == 0)
		return false;

	// Set the shader resource view
	deviceContext->VSSetShaderResources(srvInfo->BindIndex, 1, &srv);

	// Success
	return true;
}

// --------------------------------------------------------
// Sets a sampler state in the vertex shader stage
//
// name - The name of the sampler state in the shader
// samplerState - The sampler state in GPU memory
//
// Returns true if a sampler of the given name was found, false otherwise
// --------------------------------------------------------
inline bool SimpleVertexShader::SetSamplerState(std::string name, ID3D11SamplerState* samplerState)
{
	// Look for the variable and verify
	const SimpleSampler* sampInfo = GetSamplerInfo(name);
	if (sampInfo == 0)
		return false;

	// Set the shader resource view
	deviceContext->VSSetSamplers(sampInfo->BindIndex, 1, &samplerState);

	// Success
	return true;
}


///////////////////////////////////////////////////////////////////////////////
// ------ SIMPLE PIXEL SHADER -------------------------------------------------
///////////////////////////////////////////////////////////////////////////////

// --------------------------------------------------------
// Constructor just calls the base
// --------------------------------------------------------
inline SimplePixelShader::SimplePixelShader(ID3D11Device* device, ID3D11DeviceContext* context)
	: ISimpleShader(device, context) 
{ 
	this->shader = 0;
}

// --------------------------------------------------------
// Destructor - Clean up actual shader (base will be called automatically)
// --------------------------------------------------------
inline SimplePixelShader::~SimplePixelShader()
{
	CleanUp();
}

// --------------------------------------------------------
// Handles cleaning up shader and base class clean up
// --------------------------------------------------------
inline void SimplePixelShader::CleanUp()
{
	ISimpleShader::CleanUp();
	if (shader) { shader->Release(); shader = 0; }
}

// --------------------------------------------------------
// Creates the DirectX pixel shader
//
// shaderBlob - The shader's compiled code
//
// Returns true if shader is created correctly, false otherwise
// --------------------------------------------------------
inline bool SimplePixelShader::CreateShader(ID3DBlob* shaderBlob)
{
	// Clean up first, in the event this method is
	// called more than once on the same object
	this->CleanUp();

	// Create the shader from the blob
	HRESULT result = device->CreatePixelShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		0,
		&shader);

	// Check the result
	return (result == S_OK);
}

// --------------------------------------------------------
// Sets the pixel shader and constant buffers for
// future DirectX drawing
// --------------------------------------------------------
inline void SimplePixelShader::SetShaderAndCBs()
{
	// Is shader valid?
	if (!shaderValid) return;
	
	// Set the shader
	deviceContext->PSSetShader(shader, 0, 0);

	// Set the constant buffers
	for (unsigned int i = 0; i < constantBufferCount; i++)
	{
		// Skip "buffers" that aren't true constant buffers
		if (constantBuffers[i].Type != D3D11_CT_CBUFFER)
			continue;

		// This is a real constant buffer, so set it
		deviceContext->PSSetConstantBuffers(
			constantBuffers[i].BindIndex,
			1,
			&constantBuffers[i].ConstantBuffer);
	}
}

// --------------------------------------------------------
// Sets a shader resource view in the pixel shader stage
//
// name - The name of the texture resource in the shader
// srv - The shader resource view of the texture in GPU memory
//
// Returns true if a texture of the given name was found, false otherwise
// --------------------------------------------------------
inline bool SimplePixelShader::SetShaderResourceView(std::string name, ID3D11ShaderResourceView* srv)
{
	// Look for the variable and verify
	const SimpleSRV* srvInfo = GetShaderResourceViewInfo(name);
	if (srvInfo == 0)
		return false;

	// Set the shader resource view
	deviceContext->PSSetShaderResources(srvInfo->BindIndex, 1, &srv);

	// Success
	return true;
}

// --------------------------------------------------------
// Sets a sampler state in the pixel shader stage
//
// name - The name of the sampler state in the shader
// samplerState - The sampler state in GPU memory
//
// Returns true if a sampler of the given name was found, false otherwise
// --------------------------------------------------------
inline bool SimplePixelShader::SetSamplerState(std::string name, ID3D11SamplerState* samplerState)
{
	// Look for the variable and verify
	const SimpleSampler* sampInfo = GetSamplerInfo(name);
	if (sampInfo == 0)
		return false;

	// Set the shader resource view
	deviceContext->PSSetSamplers(sampInfo->BindIndex, 1, &samplerState);

	// Success
	return true;
}




///////////////////////////////////////////////////////////////////////////////
// ------ SIMPLE DOMAIN SHADER ------------------------------------------------
///////////////////////////////////////////////////////////////////////////////

// --------------------------------------------------------
// Constructor just calls the base
// --------------------------------------------------------
inline SimpleDomainShader::SimpleDomainShader(ID3D11Device* device, ID3D11DeviceContext* context)
	: ISimpleShader(device, context) 
{ 
	this->shader = 0;
}

// --------------------------------------------------------
// Destructor - Clean up actual shader (base will be called automatically)
// --------------------------------------------------------
inline SimpleDomainShader::~SimpleDomainShader()
{
	CleanUp();
}

// --------------------------------------------------------
// Handles cleaning up shader and base class clean up
// --------------------------------------------------------
inline void SimpleDomainShader::CleanUp()
{
	ISimpleShader::CleanUp();
	if (shader) { shader->Release(); shader = 0; }
}

// --------------------------------------------------------
// Creates the DirectX domain shader
//
// shaderBlob - The shader's compiled code
//
// Returns true if shader is created correctly, false otherwise
// --------------------------------------------------------
inline bool SimpleDomainShader::CreateShader(ID3DBlob* shaderBlob)
{
	// Clean up first, in the event this method is
	// called more than once on the same object
	this->CleanUp();

	// Create the shader from the blob
	HRESULT result = device->CreateDomainShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		0,
		&shader);

	// Check the result
	return (result == S_OK);
}

// --------------------------------------------------------
// Sets the domain shader and constant buffers for
// future DirectX drawing
// --------------------------------------------------------
inline void SimpleDomainShader::SetShaderAndCBs()
{
	// Is shader valid?
	if (!shaderValid) return;

	// Set the shader
	deviceContext->DSSetShader(shader, 0, 0);

	// Set the constant buffers
	for (unsigned int i = 0; i < constantBufferCount; i++)
	{
		// Skip "buffers" that aren't true constant buffers
		if (constantBuffers[i].Type != D3D11_CT_CBUFFER)
			continue;

		// This is a real constant buffer, so set it
		deviceContext->DSSetConstantBuffers(
			constantBuffers[i].BindIndex,
			1,
			&constantBuffers[i].ConstantBuffer);
	}
}

// --------------------------------------------------------
// Sets a shader resource view in the domain shader stage
//
// name - The name of the texture resource in the shader
// srv - The shader resource view of the texture in GPU memory
//
// Returns true if a texture of the given name was found, false otherwise
// --------------------------------------------------------
inline bool SimpleDomainShader::SetShaderResourceView(std::string name, ID3D11ShaderResourceView* srv)
{
	// Look for the variable and verify
	const SimpleSRV* srvInfo = GetShaderResourceViewInfo(name);
	if (srvInfo == 0)
		return false;

	// Set the shader resource view
	deviceContext->DSSetShaderResources(srvInfo->BindIndex, 1, &srv);

	// Success
	return true;
}

// --------------------------------------------------------
// Sets a sampler state in the domain shader stage
//
// name - The name of the sampler state in the shader
// samplerState - The sampler state in GPU memory
//
// Returns true if a sampler of the given name was found, false otherwise
// --------------------------------------------------------
inline bool SimpleDomainShader::SetSamplerState(std::string name, ID3D11SamplerState* samplerState)
{
	// Look for the variable and verify
	const SimpleSampler* sampInfo = GetSamplerInfo(name);
	if (sampInfo == 0)
		return false;

	// Set the shader resource view
	deviceContext->DSSetSamplers(sampInfo->BindIndex, 1, &samplerState);

	// Success
	return true;
}



///////////////////////////////////////////////////////////////////////////////
// ------ SIMPLE HULL SHADER --------------------------------------------------
///////////////////////////////////////////////////////////////////////////////

// --------------------------------------------------------
// Constructor just calls the base
// --------------------------------------------------------
inline SimpleHullShader::SimpleHullShader(ID3D11Device* device, ID3D11DeviceContext* context)
	: ISimpleShader(device, context) 
{ 
	this->shader = 0;
}

// --------------------------------------------------------
// Destructor - Clean up actual shader (base will be called automatically)
// --------------------------------------------------------
inline SimpleHullShader::~SimpleHullShader()
{
	CleanUp();
}

// --------------------------------------------------------
// Handles cleaning up shader and base class clean up
// --------------------------------------------------------
inline void SimpleHullShader::CleanUp()
{
	ISimpleShader::CleanUp();
	if (shader) { shader->Release(); shader = 0; }
}

// --------------------------------------------------------
// Creates the DirectX hull shader
//
// shaderBlob - The shader's compiled code
//
// Returns true if shader is created correctly, false otherwise
// --------------------------------------------------------
inline bool SimpleHullShader::CreateShader(ID3DBlob* shaderBlob)
{
	// Clean up first, in the event this method is
	// called more than once on the same object
	this->CleanUp();

	// Create the shader from the blob
	HRESULT result = device->CreateHullShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		0,
		&shader);

	// Check the result
	return (result == S_OK);
}

// --------------------------------------------------------
// Sets the hull shader and constant buffers for
// future DirectX drawing
// --------------------------------------------------------
inline void SimpleHullShader::SetShaderAndCBs()
{
	// Is shader valid?
	if (!shaderValid) return;

	// Set the shader
	deviceContext->HSSetShader(shader, 0, 0);

	// Set the constant buffers?
	for (unsigned int i = 0; i < constantBufferCount; i++)
	{
		// Skip "buffers" that aren't true constant buffers
		if (constantBuffers[i].Type != D3D11_CT_CBUFFER)
			continue;

		// This is a real constant buffer, so set it
		deviceContext->HSSetConstantBuffers(
			constantBuffers[i].BindIndex,
			1,
			&constantBuffers[i].ConstantBuffer);
	}
}

// --------------------------------------------------------
// Sets a shader resource view in the hull shader stage
//
// name - The name of the texture resource in the shader
// srv - The shader resource view of the texture in GPU memory
//
// Returns true if a texture of the given name was found, false otherwise
// --------------------------------------------------------
inline bool SimpleHullShader::SetShaderResourceView(std::string name, ID3D11ShaderResourceView* srv)
{
	// Look for the variable and verify
	const SimpleSRV* srvInfo = GetShaderResourceViewInfo(name);
	if (srvInfo == 0)
		return false;

	// Set the shader resource view
	deviceContext->HSSetShaderResources(srvInfo->BindIndex, 1, &srv);

	// Success
	return true;
}

// --------------------------------------------------------
// Sets a sampler state in the hull shader stage
//
// name - The name of the sampler state in the shader
// samplerState - The sampler state in GPU memory
//
// Returns true if a sampler of the given name was found, false otherwise
// --------------------------------------------------------
inline bool SimpleHullShader::SetSamplerState(std::string name, ID3D11SamplerState* samplerState)
{
	// Look for the variable and verify
	const SimpleSampler* sampInfo = GetSamplerInfo(name);
	if (sampInfo == 0)
		return false;

	// Set the shader resource view
	deviceContext->HSSetSamplers(sampInfo->BindIndex, 1, &samplerState);

	// Success
	return true;
}




///////////////////////////////////////////////////////////////////////////////
// ------ SIMPLE GEOMETRY SHADER ----------------------------------------------
///////////////////////////////////////////////////////////////////////////////

// --------------------------------------------------------
// Constructor calls the base and sets up potential stream-out options
// --------------------------------------------------------
inline SimpleGeometryShader::SimpleGeometryShader(ID3D11Device* device, ID3D11DeviceContext* context, bool useStreamOut, bool allowStreamOutRasterization)
	: ISimpleShader(device, context) 
{ 
	this->shader = 0;
	this->useStreamOut = useStreamOut;
	this->allowStreamOutRasterization = allowStreamOutRasterization;
}

// --------------------------------------------------------
// Destructor - Clean up actual shader (base will be called automatically)
// --------------------------------------------------------
inline SimpleGeometryShader::~SimpleGeometryShader()
{
	CleanUp();
}

// --------------------------------------------------------
// Handles cleaning up shader and base class clean up
// --------------------------------------------------------
inline void SimpleGeometryShader::CleanUp()
{
	ISimpleShader::CleanUp();
	if (shader) { shader->Release(); shader = 0; }
}

// --------------------------------------------------------
// Creates the DirectX Geometry shader
//
// shaderBlob - The shader's compiled code
//
// Returns true if shader is created correctly, false otherwise
// --------------------------------------------------------
inline bool SimpleGeometryShader::CreateShader(ID3DBlob* shaderBlob)
{
	// Clean up first, in the event this method is
	// called more than once on the same object
	this->CleanUp();

	// Using stream out?
	if (useStreamOut)
		return this->CreateShaderWithStreamOut(shaderBlob);

	// Create the shader from the blob
	HRESULT result = device->CreateGeometryShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		0,
		&shader);

	// Check the result
	return (result == S_OK);
}

// --------------------------------------------------------
// Creates the DirectX Geometry shader and sets it up for
// stream output, if possible.
//
// shaderBlob - The shader's compiled code
//
// Returns true if shader is created correctly, false otherwise
// --------------------------------------------------------
inline bool SimpleGeometryShader::CreateShaderWithStreamOut(ID3DBlob* shaderBlob)
{
	// Clean up first, in the event this method is
	// called more than once on the same object
	this->CleanUp();

	// Reflect shader info
	ID3D11ShaderReflection* refl;
	D3DReflect(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		IID_ID3D11ShaderReflection,
		(void**)&refl);

	// Get shader info
	D3D11_SHADER_DESC shaderDesc;
	refl->GetDesc(&shaderDesc);

	// Set up the output signature
	streamOutVertexSize = 0;
	std::vector<D3D11_SO_DECLARATION_ENTRY> soDecl;
	for (unsigned int i = 0; i < shaderDesc.OutputParameters; i++)
	{
		// Get the info about this entry
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		refl->GetOutputParameterDesc(i, &paramDesc);
		
		// Create the SO Declaration
		D3D11_SO_DECLARATION_ENTRY entry;
		entry.SemanticIndex  = paramDesc.SemanticIndex;
		entry.SemanticName   = paramDesc.SemanticName;
		entry.Stream         = paramDesc.Stream;
		entry.StartComponent = 0; // Assume starting at 0
		entry.OutputSlot     = 0; // Assume the first output slot

		// Check the mask to determine how many components are used
		entry.ComponentCount = CalcComponentCount(paramDesc.Mask);
	
		// Increment the size
		streamOutVertexSize += entry.ComponentCount * sizeof(float);

		// Add to the declaration
		soDecl.push_back(entry);
	}

	// Rasterization allowed?
	unsigned int rast = allowStreamOutRasterization ? 0 : D3D11_SO_NO_RASTERIZED_STREAM;

	// Create the shader
	HRESULT result = device->CreateGeometryShaderWithStreamOutput(
		shaderBlob->GetBufferPointer(), // Shader blob pointer
		shaderBlob->GetBufferSize(),    // Shader blob size
		&soDecl[0],                     // Stream out declaration
		(unsigned int)soDecl.size(),    // Number of declaration entries
		NULL,                           // Buffer strides (not used - assume tightly packed?)
		0,                              // No buffer strides
		rast,                           // Index of the stream to rasterize (if any)
		NULL,                           // Not using class linkage
		&shader);
	
	return (result == S_OK);
}

// --------------------------------------------------------
// Creates a vertex buffer that is compatible with the stream output
// delcaration that was used to create the shader.  This buffer will
// not be cleaned up (Released) by the simple shader - you must clean
// it up yourself when you're done with it.  Immediately returns
// false if the shader was not created with stream output, the shader
// isn't valid or the determined stream out vertex size is zero.
//
// buffer - Pointer to an ID3D11Buffer pointer to hold the buffer ref
// vertexCount - Amount of vertices the buffer should hold
//
// Returns true if buffer is created successfully AND stream output
// was used to create the shader.  False otherwise.
// --------------------------------------------------------
inline bool SimpleGeometryShader::CreateCompatibleStreamOutBuffer(ID3D11Buffer** buffer, int vertexCount)
{
	// Was stream output actually used?
	if (!this->useStreamOut || !shaderValid || streamOutVertexSize == 0)
		return false;

	// Set up the buffer description
	D3D11_BUFFER_DESC desc;
	desc.BindFlags           = D3D11_BIND_STREAM_OUTPUT | D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth           = streamOutVertexSize * vertexCount;
	desc.CPUAccessFlags      = 0;
	desc.MiscFlags           = 0;
	desc.StructureByteStride = 0;
	desc.Usage               = D3D11_USAGE_DEFAULT;

	// Attempt to create the buffer and return the result
	HRESULT result = device->CreateBuffer(&desc, 0, buffer);
	return (result == S_OK);
}

// --------------------------------------------------------
// Helper method to unbind all stream out buffers from the SO stage
// --------------------------------------------------------
inline void SimpleGeometryShader::UnbindStreamOutStage(ID3D11DeviceContext* deviceContext)
{
	unsigned int offset = 0;
	ID3D11Buffer* unset[1] = { 0 };
	deviceContext->SOSetTargets(1, unset, &offset);
}

// --------------------------------------------------------
// Sets the geometry shader and constant buffers for
// future DirectX drawing
// --------------------------------------------------------
inline void SimpleGeometryShader::SetShaderAndCBs()
{
	// Is shader valid?
	if (!shaderValid) return;

	// Set the shader
	deviceContext->GSSetShader(shader, 0, 0);

	// Set the constant buffers?
	for (unsigned int i = 0; i < constantBufferCount; i++)
	{
		// Skip "buffers" that aren't true constant buffers
		if (constantBuffers[i].Type != D3D11_CT_CBUFFER)
			continue;

		// This is a real constant buffer, so set it
		deviceContext->GSSetConstantBuffers(
			constantBuffers[i].BindIndex,
			1,
			&constantBuffers[i].ConstantBuffer);
	}
}

// --------------------------------------------------------
// Sets a shader resource view in the Geometry shader stage
//
// name - The name of the texture resource in the shader
// srv - The shader resource view of the texture in GPU memory
//
// Returns true if a texture of the given name was found, false otherwise
// --------------------------------------------------------
inline bool SimpleGeometryShader::SetShaderResourceView(std::string name, ID3D11ShaderResourceView* srv)
{
	// Look for the variable and verify
	const SimpleSRV* srvInfo = GetShaderResourceViewInfo(name);
	if (srvInfo == 0)
		return false;

	// Set the shader resource view
	deviceContext->GSSetShaderResources(srvInfo->BindIndex, 1, &srv);

	// Success
	return true;
}

// --------------------------------------------------------
// Sets a sampler state in the Geometry shader stage
//
// name - The name of the sampler state in the shader
// samplerState - The sampler state in GPU memory
//
// Returns true if a sampler of the given name was found, false otherwise
// --------------------------------------------------------
inline bool SimpleGeometryShader::SetSamplerState(std::string name, ID3D11SamplerState* samplerState)
{
	// Look for the variable and verify
	const SimpleSampler* sampInfo = GetSamplerInfo(name);
	if (sampInfo == 0)
		return false;

	// Set the shader resource view
	deviceContext->GSSetSamplers(sampInfo->BindIndex, 1, &samplerState);

	// Success
	return true;
}

// --------------------------------------------------------
// Calculates the number of components specified by a parameter description mask
//
// mask - The mask to check (only values 0 - 15 are considered)
//
// Returns an integer between 0 - 4 inclusive
// --------------------------------------------------------
inline unsigned int SimpleGeometryShader::CalcComponentCount(unsigned int mask)
{
	unsigned int result = 0;
	result += (unsigned int)((mask & 1) == 1);
	result += (unsigned int)((mask & 2) == 2);
	result += (unsigned int)((mask & 4) == 4);
	result += (unsigned int)((mask & 8) == 8);
	return result;
}



///////////////////////////////////////////////////////////////////////////////
// ------ SIMPLE COMPUTE SHADER -----------------------------------------------
///////////////////////////////////////////////////////////////////////////////

// --------------------------------------------------------
// Constructor just calls the base
// --------------------------------------------------------
inline SimpleComputeShader::SimpleComputeShader(ID3D11Device* device, ID3D11DeviceContext* context)
	: ISimpleShader(device, context) 
{ 
	this->shader = 0;
}

// --------------------------------------------------------
// Destructor - Clean up actual shader (base will be called automatically)
// --------------------------------------------------------
inline SimpleComputeShader::~SimpleComputeShader()
{
	CleanUp();
}

// --------------------------------------------------------
// Handles cleaning up shader and base class clean up
// --------------------------------------------------------
inline void SimpleComputeShader::CleanUp()
{
	ISimpleShader::CleanUp();
	if (shader) { shader->Release(); shader = 0; }

	uavTable.clear();
}

// --------------------------------------------------------
// Creates the DirectX Compute shader
//
// shaderBlob - The shader's compiled code
//
// Returns true if shader is created correctly, false otherwise
// --------------------------------------------------------
inline bool SimpleComputeShader::CreateShader(ID3DBlob* shaderBlob)
{
	// Clean up first, in the event this method is
	// called more than once on the same object
	this->CleanUp();

	// Create the shader from the blob
	HRESULT result = device->CreateComputeShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		0,
		&shader);

	// Was the shader created correctly?
	if (result != S_OK)
		return false;

	// Set up shader reflection to get information about UAV's
	ID3D11ShaderReflection* refl;
	D3DReflect(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		IID_ID3D11ShaderReflection,
		(void**)&refl);

	// Get the description of the shader
	D3D11_SHADER_DESC shaderDesc;
	refl->GetDesc(&shaderDesc);
	
	// Grab the thread info
	threadsTotal = refl->GetThreadGroupSize(
		&threadsX,
		&threadsY,
		&threadsZ);

	// Loop and get all UAV resources
	unsigned int resourceCount = shaderDesc.BoundResources;
	for (unsigned int r = 0; r < resourceCount; r++)
	{
		// Get this resource's description
		D3D11_SHADER_INPUT_BIND_DESC resourceDesc;
		refl->GetResourceBindingDesc(r, &resourceDesc);

		// Check the type, looking for any kind of UAV
		switch (resourceDesc.Type)
		{
		case D3D_SIT_UAV_APPEND_STRUCTURED:
		case D3D_SIT_UAV_CONSUME_STRUCTURED:
		case D3D_SIT_UAV_RWBYTEADDRESS:
		case D3D_SIT_UAV_RWSTRUCTURED:
		case D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER:
		case D3D_SIT_UAV_RWTYPED:
			uavTable.insert(std::pair<std::string, unsigned int>(resourceDesc.Name, resourceDesc.BindPoint));
		}
	}

	// All set
	refl->Release();
	return true;
}

// --------------------------------------------------------
// Sets the Compute shader and constant buffers for
// future DirectX drawing
// --------------------------------------------------------
inline void SimpleComputeShader::SetShaderAndCBs()
{
	// Is shader valid?
	if (!shaderValid) return;

	// Set the shader
	deviceContext->CSSetShader(shader, 0, 0);

	// Set the constant buffers?
	for (unsigned int i = 0; i < constantBufferCount; i++)
	{
		// Skip "buffers" that aren't true constant buffers
		if (constantBuffers[i].Type != D3D11_CT_CBUFFER)
			continue;

		// This is a real constant buffer, so set it
		deviceContext->CSSetConstantBuffers(
			constantBuffers[i].BindIndex,
			1,
			&constantBuffers[i].ConstantBuffer);
	}
}

// --------------------------------------------------------
// Dispatches the compute shader with the specified amount 
// of groups, using the number of threads per group
// specified in the shader file itself
//
// For example, calling this method with params (5,1,1) on
// a shader with (8,2,2) threads per group will launch a 
// total of 160 threads: ((5 * 8) * (1 * 2) * (1 * 2))
//
// This is identical to using the device context's 
// Dispatch() method yourself.  
//
// Note: This will dispatch the currently active shader, 
// not necessarily THIS shader. Be sure to activate this
// shader with SetShader() before calling Dispatch
//
// groupsX - Numbers of groups in the X dimension
// groupsY - Numbers of groups in the Y dimension
// groupsZ - Numbers of groups in the Z dimension
// --------------------------------------------------------
inline void SimpleComputeShader::DispatchByGroups(unsigned int groupsX, unsigned int groupsY, unsigned int groupsZ)
{
	deviceContext->Dispatch(groupsX, groupsY, groupsZ);
}

// --------------------------------------------------------
// Dispatches the compute shader with AT LEAST the 
// specified amount of threads, calculating the number of
// groups to dispatch using the number of threads per group
// specified in the shader file itself
//
// For example, calling this method with params (10,3,3) on
// a shader with (5,2,2) threads per group will launch 
// 8 total groups and 160 total threads, calculated by:
// Groups: ceil(10/5) * ceil(3/2) * ceil(3/2) = 8
// Threads: ((2 * 5) * (2 * 2) * (2 * 2)) = 160
//
// Note: This will dispatch the currently active shader, 
// not necessarily THIS shader. Be sure to activate this
// shader with SetShader() before calling Dispatch
//
// threadsX - Desired numbers of threads in the X dimension
// threadsY - Desired numbers of threads in the Y dimension
// threadsZ - Desired numbers of threads in the Z dimension
// --------------------------------------------------------
inline void SimpleComputeShader::DispatchByThreads(unsigned int threadsX, unsigned int threadsY, unsigned int threadsZ)
{
	deviceContext->Dispatch(
		max((unsigned int)ceil((float)threadsX / this->threadsX), 1),
		max((unsigned int)ceil((float)threadsY / this->threadsY), 1),
		max((unsigned int)ceil((float)threadsZ / this->threadsZ), 1));
}

// --------------------------------------------------------
// Sets a shader resource view in the Compute shader stage
//
// name - The name of the texture resource in the shader
// srv - The shader resource view of the texture in GPU memory
//
// Returns true if a texture of the given name was found, false otherwise
// --------------------------------------------------------
inline bool SimpleComputeShader::SetShaderResourceView(std::string name, ID3D11ShaderResourceView* srv)
{
	// Look for the variable and verify
	const SimpleSRV* srvInfo = GetShaderResourceViewInfo(name);
	if (srvInfo == 0)
		return false;

	// Set the shader resource view
	deviceContext->CSSetShaderResources(srvInfo->BindIndex, 1, &srv);

	// Success
	return true;
}

// --------------------------------------------------------
// Sets a sampler state in the Compute shader stage
//
// name - The name of the sampler state in the shader
// samplerState - The sampler state in GPU memory
//
// Returns true if a sampler of the given name was found, false otherwise
// --------------------------------------------------------
inline bool SimpleComputeShader::SetSamplerState(std::string name, ID3D11SamplerState* samplerState)
{
	// Look for the variable and verify
	const SimpleSampler* sampInfo = GetSamplerInfo(name);
	if (sampInfo == 0)
		return false;

	// Set the shader resource view
	deviceContext->CSSetSamplers(sampInfo->BindIndex, 1, &samplerState);

	// Success
	return true;
}

// --------------------------------------------------------
// Sets an unordered access view in the Compute shader stage
//
// name - The name of the sampler state in the shader
// uav - The UAV in GPU memory
// appendConsumeOffset - Used for append or consume UAV's (optional)
//
// Returns true if a UAV of the given name was found, false otherwise
// --------------------------------------------------------
inline bool SimpleComputeShader::SetUnorderedAccessView(std::string name, ID3D11UnorderedAccessView * uav, unsigned int appendConsumeOffset)
{
	// Look for the variable and verify
	unsigned int bindIndex = GetUnorderedAccessViewIndex(name);
	if (bindIndex == -1)
		return false;

	// Set the shader resource view
	deviceContext->CSSetUnorderedAccessViews(bindIndex, 1, &uav, &appendConsumeOffset);

	// Success
	return true;
}

// --------------------------------------------------------
// Gets the index of the specified UAV (or -1)
// --------------------------------------------------------
inline int SimpleComputeShader::GetUnorderedAccessViewIndex(std::string name)
{
	// Look for the key
	std::unordered_map<std::string, unsigned int>::iterator result =
		uavTable.find(name);

	// Did we find the key?
	if (result == uavTable.end())
		return -1;

	// Success
	return result->second;
}

#undef min
#undef max
#pragma once
#include <wrl.h>
#include <DirectXMath.h>
#include <d3d12.h>
#include"DirectionLight.h"
#include"PointLight.h"

class LightGroup
{
private:
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: 
	static const int DirLightNum = 3;
	static const int PointLightNum = 3;

public: 

	
	struct ConstBufferData
	{
		
		XMFLOAT3 ambientColor;
		float pad1;
		DirectionLight::ConstBufferData dirLights[DirLightNum];
		PointLight::ConstBufferData pointLights[PointLightNum];
	};

public: 
	static void StaticInitialize(ID3D12Device* device);

	
	static LightGroup* Create();

private: 
	static ID3D12Device* device;

public: 
	void Initialize();

	
	void Update();

	
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndex);

	
	void TransferConstBuffer();

	
	void DefaultLightSetting();

	
	void SetAmbientColor(const XMFLOAT3& color);

	
	void SetDirLightActive(int index, bool active);

	
	void SetDirLightDir(int index, const XMVECTOR& lightdir);

	
	void SetDirLightColor(int index, const XMFLOAT3& lightcolor);

	void SetPointLightActive(int index, bool active);

	void SetPointLightPos(int index, const XMFLOAT3& lightpos);

	void SetPointLightColor(int index, const XMFLOAT3& lightcolor);

	void SetPointLightAtten(int index, const XMFLOAT3& lightatten);
private: 
	ComPtr<ID3D12Resource> constBuff;

	
	XMFLOAT3 ambientColor = { 1,1,1 };

	
	DirectionLight dirLights[DirLightNum];
	PointLight pointLights[PointLightNum];

	
	bool dirty = false;
};


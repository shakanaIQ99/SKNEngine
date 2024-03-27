#pragma once
#include <wrl.h>
#include <d3d12.h>
#include"DirectionLight.h"
#include"PointLight.h"

class LightGroup
{
private:
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

public: 
	static const int DirLightNum = 1;
	static const int PointLightNum = 1;

public: 

	
	struct ConstBufferData
	{
		
		Vector3 ambientColor;
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

	
	void SetAmbientColor(const Vector3& color);

	
	void SetDirLightActive(int index, bool active);

	
	void SetDirLightDir(int index, const Vector3& lightdir);

	
	void SetDirLightColor(int index, const Vector3& lightcolor);

	void SetPointLightActive(int index, bool active);

	void SetPointLightPos(int index, const Vector3& lightpos);

	void SetPointLightColor(int index, const Vector3& lightcolor);

	void SetPointLightAtten(int index, const Vector3& lightatten);
private: 
	ComPtr<ID3D12Resource> constBuff;

	
	Vector3 ambientColor = { 0.5f,0.5f,0.5f };

	
	DirectionLight dirLights[DirLightNum];
	PointLight pointLights[PointLightNum];

	
	bool dirty = false;
};


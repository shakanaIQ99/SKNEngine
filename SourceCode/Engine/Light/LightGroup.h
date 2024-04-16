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

	
	struct ConstBufferData
	{
		
		Vector3 ambientColor;
		float pad;
		DirectionLight::ConstBufferData dirLights;
		PointLight::ConstBufferData pointLights;
	};

public: 

	static LightGroup* Create();

public: 
	void Initialize();

	
	void Update();

	
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndex);

	
	void TransferConstBuffer();

	
	void DefaultLightSetting();

	
	void SetAmbientColor(const Vector3& color);

	
	void SetDirLightActive(bool active);

	
	void SetDirLightDir(const Vector3& lightdir);

	
	void SetDirLightColor(const Vector3& lightcolor);

	void SetPointLightActive( bool active);

	void SetPointLightPos(const Vector3& lightpos);

	void SetPointLightColor(const Vector3& lightcolor);

	void SetPointLightAtten(const Vector3& lightatten);
private: 
	ComPtr<ID3D12Resource> constBuff;

	
	Vector3 ambientColor = {1.0f,1.0f,1.0f };

	
	DirectionLight dirLights;
	PointLight pointLights;

	
	bool dirty = false;
};


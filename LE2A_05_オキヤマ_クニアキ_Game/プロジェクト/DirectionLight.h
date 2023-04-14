#pragma once
#include <wrl.h>
#include <DirectXMath.h>
#include <d3d12.h>


class DirectionLight
{
private:

	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	struct ConstBufferData
	{
		XMVECTOR lightv;
		XMFLOAT3 lightcolor;
		unsigned int active;
	};

	static void StaticInitialize(ID3D12Device* device);

	static DirectionLight* Create();

	void Initialize();

	void TransferConstBuffer();

	void Update();

	void Draw(ID3D12GraphicsCommandList*cmdlist,UINT rootParamIndex);

	void SetLightDir(const XMVECTOR& lightdir);

	void SetLightColor(const XMFLOAT3& lightcolor);

	XMVECTOR GetLightDir() { return lightdir; }

	XMFLOAT3 GetLightColor() { return lightcolor; }

	inline void SetActive(bool active) { this->active = active; }
	inline bool IsActive() { return active; }

private:



	static ID3D12Device* device;

	ComPtr<ID3D12Resource> constBuff;

	XMVECTOR lightdir = { 1,0,0,0 };

	XMFLOAT3 lightcolor = { 1,1,1 };

	bool dirty = false;

	bool active = false;

};


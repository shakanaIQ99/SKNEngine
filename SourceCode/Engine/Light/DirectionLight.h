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

	static void StaticInitialize(ID3D12Device* _device);

	static DirectionLight* Create();

	void Initialize();

	void TransferConstBuffer();

	void Update();

	void Draw(ID3D12GraphicsCommandList*cmdlist,UINT rootParamIndex);

	void SetLightDir(const XMVECTOR& _lightdir);

	void SetLightColor(const XMFLOAT3& _lightcolor);

	XMVECTOR GetLightDir() { return lightdir; }

	XMFLOAT3 GetLightColor() { return lightcolor; }

	inline void SetActive(bool Active) { this->active = Active; }
	inline bool IsActive() { return active; }

private:



	static ID3D12Device* device;

	ComPtr<ID3D12Resource> constBuff;

	DirectX::XMVECTORF32 floatingVector = { 1.0f, 0, 0, 0 };
	XMVECTOR lightdir = floatingVector;

	XMFLOAT3 lightcolor = { 1,1,1 };

	bool dirty = false;

	bool active = false;

};


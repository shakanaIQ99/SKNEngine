#pragma once
#include <wrl.h>
#include"Matrix4.h"
#include"Vector2.h"
#include"Vector3.h"
#include"Float4.h"
#include <d3d12.h>


class DirectionLight
{
private:

	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	struct ConstBufferData
	{
		Vector3 lightv;
		Vector3 lightcolor;
		unsigned int active;
	};

	static DirectionLight* Create();

	void Initialize();

	void TransferConstBuffer();

	void Update();

	void Draw(ID3D12GraphicsCommandList*cmdlist,UINT rootParamIndex);

	void SetLightDir(const Vector3& _lightdir);

	void SetLightColor(const Vector3& _lightcolor);

	Vector3 GetLightDir() { return lightdir; }

	Vector3 GetLightColor() { return lightcolor; }

	inline void SetActive(bool Active) { active = Active; }
	inline bool IsActive() { return active; }

private:

	ComPtr<ID3D12Resource> constBuff;

	Vector3 lightdir = { 1,0,0};

	Vector3 lightcolor = { 1,1,1 };

	bool dirty = false;

	bool active = false;

};


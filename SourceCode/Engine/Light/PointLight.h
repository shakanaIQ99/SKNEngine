#pragma once
//点光源
#include <DirectXMath.h>

class PointLight
{
private:

	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	struct ConstBufferData
	{
		XMFLOAT3 lightpos;
		float pad1;
		XMFLOAT3 lightcolor;
		float pad2;
		XMFLOAT3 lightatten;
		unsigned int active;
	};

	/// <summary>
	/// 座標設定
	/// </summary>
	/// <param name="_lightpos"></param>
	inline void SetLightPos(const XMFLOAT3& _lightpos) { lightpos = _lightpos; }

	/// <summary>
	/// 座標取得
	/// </summary>
	/// <returns></returns>
	inline const XMFLOAT3& GetLightPos() { return lightpos; }

	/// <summary>
	/// ライトカラー
	/// </summary>
	/// <param name="_lightcolor"></param>
	inline void SetLightColor(const XMFLOAT3& _lightcolor) { lightcolor = _lightcolor; }

	/// <summary>
	/// ライトカラー取得
	/// </summary>
	/// <returns></returns>
	inline const XMFLOAT3& GetLightColor() { return lightcolor; }

	/// <summary>
	/// 減衰率設定
	/// </summary>
	/// <param name="_lightatten"></param>
	inline void SetLightAtten(const XMFLOAT3& _lightatten) { lightatten = _lightatten; }

	/// <summary>
	/// 減衰率取得
	/// </summary>
	/// <returns></returns>
	inline const XMFLOAT3& GetLightAtten() { return lightatten; }

	/// <summary>
	/// アクティブセット
	/// </summary>
	/// <param name="Active"></param>
	inline void SetActive(bool Active) { this->active = Active; }

	/// <summary>
	/// アクティブゲット
	/// </summary>
	/// <returns></returns>
	inline bool IsActive() { return active; }

private:

	XMFLOAT3 lightpos = { 0,0,0 };
	XMFLOAT3 lightcolor = { 1,1,1 };
	XMFLOAT3 lightatten = { 1.0f,1.0f,1.0f };

	bool active = false;

};


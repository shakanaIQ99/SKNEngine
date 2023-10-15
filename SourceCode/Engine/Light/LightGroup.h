#pragma once
//ライトグループ
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
	static const int DirLightNum = 1;
	static const int PointLightNum = 1;

public: 

	
	struct ConstBufferData
	{
		
		XMFLOAT3 ambientColor;
		float pad1;
		DirectionLight::ConstBufferData dirLights[DirLightNum];
		PointLight::ConstBufferData pointLights[PointLightNum];
	};

public: 
	
	/// <summary>
	/// 生成
	/// </summary>
	/// <returns></returns>
	static LightGroup* Create();

private: 


public: 
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="rootParamIndex"></param>
	void Draw(UINT rootParamIndex);

	/// <summary>
	/// 定数バッファ転送
	/// </summary>
	void TransferConstBuffer();

	/// <summary>
	/// 定位置
	/// </summary>
	void DefaultLightSetting();

	/// <summary>
	/// アンビエントカラー設定
	/// </summary>
	/// <param name="color"></param>
	void SetAmbientColor(const XMFLOAT3& color);

	/// <summary>
	/// 太陽光アクティブセット
	/// </summary>
	/// <param name="index"></param>
	/// <param name="active"></param>
	void SetDirLightActive(int index, bool active);

	/// <summary>
	/// 太陽光の向き設定
	/// </summary>
	/// <param name="index"></param>
	/// <param name="lightdir"></param>
	void SetDirLightDir(int index, const XMVECTOR& lightdir);

	/// <summary>
	/// 太陽光の色設定
	/// </summary>
	/// <param name="index"></param>
	/// <param name="lightcolor"></param>
	void SetDirLightColor(int index, const XMFLOAT3& lightcolor);

	/// <summary>
	/// 点光源アクティブセット
	/// </summary>
	/// <param name="index"></param>
	/// <param name="active"></param>
	void SetPointLightActive(int index, bool active);

	/// <summary>
	/// 点光源座標設定
	/// </summary>
	/// <param name="index"></param>
	/// <param name="lightpos"></param>
	void SetPointLightPos(int index, const XMFLOAT3& lightpos);

	/// <summary>
	/// 点光源の色設定
	/// </summary>
	/// <param name="index"></param>
	/// <param name="lightcolor"></param>
	void SetPointLightColor(int index, const XMFLOAT3& lightcolor);

	/// <summary>
	/// 点光源の減衰率設定
	/// </summary>
	/// <param name="index"></param>
	/// <param name="lightatten"></param>
	void SetPointLightAtten(int index, const XMFLOAT3& lightatten);
private: 
	ComPtr<ID3D12Resource> constBuff;

	
	XMFLOAT3 ambientColor = { 1,1,1 };

	
	DirectionLight dirLights[DirLightNum];
	PointLight pointLights[PointLightNum];

	
	bool dirty = false;
};


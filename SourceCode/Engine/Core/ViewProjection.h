#pragma once
//ビュープロジェクション
#include<DirectXMath.h>

using namespace DirectX;

class ViewProjection
{
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
	/// ビュー行列更新
	/// </summary>
	void UpdateMatview();


	/// <summary>
	/// 射影行列更新
	/// </summary>
	void UpdateMatProjection();

	/// <summary>
	/// ビュー行列取得
	/// </summary>
	/// <returns></returns>
	const XMMATRIX& GetMAtView();
	/// <summary>
	/// 射影行列取得
	/// </summary>
	/// <returns></returns>
	const XMMATRIX& GetMatProjection();
	/// <summary>
	/// アイ取得
	/// </summary>
	/// <returns></returns>
	const XMFLOAT3& Geteye();
	/// <summary>
	/// ターゲット取得
	/// </summary>
	/// <returns></returns>
	const XMFLOAT3& Gettarget();
	/// <summary>
	/// アップ取得
	/// </summary>
	/// <returns></returns>
	const XMFLOAT3& Getup();
	
	/// <summary>
	/// アイを設定
	/// </summary>
	/// <param name="eye_"></param>
	void SetEye(const XMFLOAT3& eye_);
	/// <summary>
	/// ターゲット設定
	/// </summary>
	/// <param name="target_"></param>
	void SetTarget(const XMFLOAT3& target_);
	/// <summary>
	/// アップ設定
	/// </summary>
	/// <param name="up_"></param>
	void SetUp(const XMFLOAT3& up_);

private:

	XMMATRIX matView;

	XMMATRIX matProjection;

	XMFLOAT3 eye;

	XMFLOAT3 target;

	XMFLOAT3 up;

	float aspect = 0.0f;
	float_t nearClip = 0.1f;
	float_t farClip = 2000.0f;
	float angle = 0.0f;
	float distance = 50.0f;

};


#pragma once
//カメラ
#include <d3d12.h>
#include "WorldTronsform.h"
#include "ViewProjection.h"

using namespace DirectX;

class Camera
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="worldTransform">ワールド座標</param>
	/// <param name="Rot">回転角(ラジアン)</param>
	void Initialize();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 行列とベクトルの計算(左側が行列計算の先)
	/// </summary>
	XMFLOAT3 VectorMat(XMFLOAT3 vector, XMMATRIX mat);

	/// <summary>
	/// 行列とベクトルの計算(左側が行列計算の先)
	/// </summary>
	XMFLOAT3 VectorMat(XMMATRIX mat, XMFLOAT3 vector);

	/// <summary>
	/// ビュー行列取得
	/// </summary>
	/// <returns></returns>
	ViewProjection* getView();

	/// <summary>
	/// ワールド行列
	/// </summary>
	/// <returns></returns>
	XMMATRIX getMatWorld();

	/// <summary>
	/// 正面ベクトル取得
	/// </summary>
	/// <returns></returns>
	XMFLOAT3 getForwardVec();

	//ワールド座標を取得
	XMFLOAT3 GetWorldPosition();

	/// <summary>
	/// 座標設定
	/// </summary>
	/// <param name="pos"></param>
	void setPos(XMFLOAT3 pos);

	/// <summary>
	/// ターゲット設定
	/// </summary>
	/// <param name="Target"></param>
	void setTarget(XMFLOAT3 Target);

	/// <summary>
	/// カメラに追従先のトランスフォームを渡す
	/// </summary>
	void setTarget(WorldTransform* target) { targetWT = target; }

	/// <summary>
	/// 回転設定
	/// </summary>
	/// <param name="rotate"></param>
	void setRotate(XMFLOAT3 rotate);


	/// <summary>
	/// ワールド行列設定
	/// </summary>
	/// <param name="woeldMat"></param>
	void setWorldMat(XMMATRIX woeldMat);

private:

	const WorldTransform* targetWT = nullptr;

	ViewProjection viewProjection_;

	//ワールド変換データ(適当読み込み)
	WorldTransform wt;

	//ビュープロジェクション

	XMFLOAT3 forward = { 0.0f, 0.0f, 1.0f };

	float cameraRotateX = 0;
	float cameraRotateY = 0;
	float rotateX = 0;
	float rotateY = 0;

	const float cameraDPI = 0.05f;

};

const XMFLOAT3 operator+= (XMFLOAT3& v1, const XMFLOAT3& v2);

const XMFLOAT3 operator+(const XMFLOAT3& v1, const XMFLOAT3& v2);

float length(XMFLOAT3& a);
void normalize(XMFLOAT3& a);
#pragma once
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
	void Initialize(XMFLOAT3 worldPos,XMFLOAT3 Rot, ID3D12Device* Device);

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

	ViewProjection* getView();

	XMMATRIX getMatWorld();

	XMFLOAT3 getForwardVec();

	//ワールド座標を取得
	XMFLOAT3 GetWorldPosition();

	void setPos(XMFLOAT3 pos);

	void setTarget(XMFLOAT3 Target);

	void setRotate(XMFLOAT3 rotate);

	void setWorldMat(XMMATRIX woeldMat);

private:

	ViewProjection viewProjection_;

	//ワールド変換データ(適当読み込み)
	WorldTransform wt;

	//ビュープロジェクション

	XMFLOAT3 forward = { 0.0f, 0.0f, 1.0f };

};

const XMFLOAT3 operator+= (XMFLOAT3& v1, const XMFLOAT3& v2);

const XMFLOAT3 operator+(const XMFLOAT3& v1, const XMFLOAT3& v2);

float length(XMFLOAT3& a);
void normalize(XMFLOAT3& a);
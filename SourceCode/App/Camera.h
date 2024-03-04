#pragma once
#include <d3d12.h>
#include "WorldTronsform.h"
#include "ViewProjection.h"
#include"myMath.h"

class Camera
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="worldTransform">ワールド座標</param>
	/// <param name="Rot">回転角(ラジアン)</param>
	void Initialize(ID3D12Device* Device);

	void Reset();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	ViewProjection* getView();

	Matrix4 getMatWorld();

	Vector3 getForwardVec();



	void setPos(Vector3 pos);

	void setTarget(Vector3 Target);
	/// <summary>
	/// カメラに追従先のトランスフォームを渡す
	/// </summary>
	void setTarget(WorldTransform* target) { targetWT = target; }

	void setRotate(Vector3 rotate);

	void setWorldMat(Matrix4 woeldMat);

private:

	const WorldTransform* targetWT = nullptr;

	ViewProjection viewProjection_;

	//ワールド変換データ(適当読み込み)
	WorldTransform wt;

	//ビュープロジェクション

	Vector3 forward = { 0.0f, 0.0f, 1.0f };

	Vector3 interTarget = {};

	float cameraRotateX = 0;
	float cameraRotateY = 0;
	float rotateX = 0;
	float rotateY = 0;

	const float cameraDPI = 0.05f;

};


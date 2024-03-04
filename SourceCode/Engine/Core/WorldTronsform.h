#pragma once
#include"Matrix4.h"
#include"Vector3.h"
#include"Float4.h"
#include <d3d12.h>
#include <wrl.h>
#include"ViewProjection.h"


using namespace Microsoft::WRL;

// 定数バッファ用データ構造体
struct ConstBufferDataWorldTransformB0
{
	Matrix4 viewproj;
	Matrix4 world;
	Vector3 cameraPos;
	float pad1;
	Float4 color;
};



/// <summary>
/// ワールド変換データ
/// </summary>
struct WorldTransform {

	
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuffB0;
	// マッピング済みアドレス
	ConstBufferDataWorldTransformB0* constMap = nullptr;
	// ローカルスケール
	Vector3 scale_ = { 1, 1, 1 };
	// X,Y,Z軸回りのローカル回転角
	Vector3 rotation_ = { 0.0f, 0.0f, 0.0f };
	// ローカル座標
	Vector3 translation_ = { 0.0f, 0.0f, 0.0f };

	Float4 color = { 1,1,1,1 };
	// ローカル → ワールド変換行列
	Matrix4 matWorld_;

	Matrix4 matBillboard;
	// 親となるワールド変換へのポインタ
	//WorldTransform* parent_ = nullptr;

	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstBuffer(ID3D12Device* device);
	
	/// <summary>
	/// 行列を更新する
	/// </summary>
	void UpdateMatrix(ViewProjection* camera);

	void UpdateMatrixBill(ViewProjection* camera);

	void UpdateSpriteMatrix(Matrix4 projection);
};

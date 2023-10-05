#pragma once

#include <DirectXMath.h>
#include <d3d12.h>
#include <wrl.h>
#include"ViewProjection.h"

using namespace DirectX;

using namespace Microsoft::WRL;

// 定数バッファ用データ構造体
struct ConstBufferDataWorldTransformB0
{
	XMMATRIX viewproj;
	XMMATRIX world;
	XMFLOAT3 cameraPos;
	float pad1;
	XMFLOAT4 color;
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
	XMFLOAT3 scale_ = { 1, 1, 1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation_ = { 0.0f, 0.0f, 0.0f };
	// ローカル座標
	XMFLOAT3 translation_ = { 0.0f, 0.0f, 0.0f };

	XMFLOAT4 color = { 1,1,1,1 };
	// ローカル → ワールド変換行列
	XMMATRIX matWorld_;

	XMMATRIX matBillboard;
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

	void UpdateSpriteMatrix(XMMATRIX projection);
};

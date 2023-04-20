#include "WorldTronsform.h"



void WorldTransform::CreateConstBuffer(ID3D12Device* device)
{
	HRESULT result;
	D3D12_HEAP_PROPERTIES cbHeapProp{};

	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用

	D3D12_RESOURCE_DESC cbResourceDesc{};
	//リソース設定
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataWorldTransformB0) + 0xff) & ~0Xff;	//256バイトアライメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	
	//定数バッファの生成
	result = device->CreateCommittedResource(
		&cbHeapProp,		//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB0)
	);
	assert(SUCCEEDED(result));


}

void WorldTransform::Map()
{
	HRESULT result = constBuffB0->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));
	
}

void WorldTransform::UpdateMatrix(XMMATRIX view, XMMATRIX projection,XMFLOAT3 camerapos)
{
	XMMATRIX matScale, matRot, matTrans;

	// スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(rotation_.z);
	matRot *= XMMatrixRotationX(rotation_.x);
	matRot *= XMMatrixRotationY(rotation_.y);
	matTrans = XMMatrixTranslation(translation_.x, translation_.y, translation_.z);

	// ワールド行列の合成
	matWorld_ = XMMatrixIdentity(); // 変形をリセット
	matWorld_ *= matScale;          // ワールド行列にスケーリングを反映
	matWorld_ *= matRot;            // ワールド行列に回転を反映
	matWorld_ *= matTrans;          // ワールド行列に平行移動を反映

	//// 親行列の指定がある場合は、掛け算する
	//if (parent_) {
	//	matWorld_ *= parent_->matWorld_;
	//}

	// 定数バッファに書き込み
	constMap->color = color;
	//constMap->matWorld = matWorld_ * view * projection;
	constMap->viewproj = view * projection;
	constMap->world = matWorld_;
	constMap->cameraPos = camerapos;
	
}

void WorldTransform::UpdateMatrixBill(ViewProjection* camera)
{
	XMMATRIX matScale, matRot, matTrans;

	// スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(rotation_.z);
	matRot *= XMMatrixRotationX(rotation_.x);
	matRot *= XMMatrixRotationY(rotation_.y);
	matTrans = XMMatrixTranslation(translation_.x, translation_.y, translation_.z);

	// ワールド行列の合成
	matWorld_ = XMMatrixIdentity(); // 変形をリセット
	matWorld_ *= matScale;          // ワールド行列にスケーリングを反映
	matWorld_ *= matRot;            // ワールド行列に回転を反映
	matWorld_ *= matTrans;          // ワールド行列に平行移動を反映

	XMVECTOR eyePosition = XMLoadFloat3(&camera->Geteye());

	XMVECTOR targetPosition = XMLoadFloat3(&camera->Gettarget());

	XMVECTOR upVector = XMLoadFloat3(&camera->Getup());

	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);

	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	XMVECTOR cameraAxisX;

	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);

	cameraAxisX = XMVector3Normalize(cameraAxisX);

	XMVECTOR cameraAxisY;

	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	XMMATRIX matCameraRot;

	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);

	XMMATRIX matView = XMMatrixTranspose(matCameraRot);

	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);

	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);

	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

	matView.r[3] = translation;

	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);

	//// 親行列の指定がある場合は、掛け算する
	//if (parent_) {
	//	matWorld_ *= parent_->matWorld_;
	//}

	// 定数バッファに書き込み
	constMap->viewproj = matView * camera->GetMatProjection();
	constMap->world = matBillboard;
	constBuffB0->Unmap(0, nullptr);
	

}

void WorldTransform::UpdateSpriteMatrix(XMMATRIX projection)
{
	XMMATRIX matScale, matRot, matTrans;

	// スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(scale_.x, scale_.y, 1.0f);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(rotation_.z);
	matRot *= XMMatrixRotationX(0.0f);
	matRot *= XMMatrixRotationY(0.0f);
	matTrans = XMMatrixTranslation(translation_.x, translation_.y, 0.0f);

	// ワールド行列の合成
	matWorld_ = XMMatrixIdentity(); // 変形をリセット
	matWorld_ *= matScale;          // ワールド行列にスケーリングを反映
	matWorld_ *= matRot;            // ワールド行列に回転を反映
	matWorld_ *= matTrans;          // ワールド行列に平行移動を反映

	//// 親行列の指定がある場合は、掛け算する
	//if (parent_) {
	//    matWorld_ *= parent_->matWorld_;
	//}

	// 定数バッファに書き込み
	constMap->color = color;
	constMap->world = matWorld_ * projection;

}
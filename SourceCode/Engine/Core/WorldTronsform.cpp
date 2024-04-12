#include "WorldTronsform.h"
#include<cassert>
#include "DirectXCommon.h"


void WorldTransform::CreateConstBuffer()
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
	result = SKNEngine::DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&cbHeapProp,		//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB0)
	);
	assert(SUCCEEDED(result));


}

void WorldTransform::UpdateMatrix(ViewProjection* camera)
{
	
	matWorld_ = Matrix4();

	// ワールド行列の合成
	matWorld_ *= Matrix4::Scaling(scale_.x, scale_.y, scale_.z);						// ワールド行列にスケーリングを反映
	matWorld_ *= Matrix4::RotationZXY(rotation_.x, rotation_.y, rotation_.z);           // ワールド行列に回転を反映
	matWorld_ *= Matrix4::Translation(translation_.x, translation_.y, translation_.z);          // ワールド行列に平行移動を反映

	//// 親行列の指定がある場合は、掛け算する
	//if (parent_) {
	//	matWorld_ *= parent_->matWorld_;
	//}

	HRESULT result = constBuffB0->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));
	(void)result;
	// 定数バッファに書き込み
	constMap->color = color;
	//constMap->matWorld = matWorld_ * view * projection;
	constMap->viewproj = camera->GetMAtView() * camera->GetMatProjection();
	constMap->world = matWorld_;
	constMap->cameraPos = camera->Geteye();

	
}

void WorldTransform::UpdateMatrixBill(ViewProjection* camera)
{
	matWorld_ = Matrix4();

	// ワールド行列の合成
	matWorld_ *= Matrix4::Scaling(scale_.x, scale_.y, scale_.z);						// ワールド行列にスケーリングを反映
	matWorld_ *= Matrix4::RotationZXY(rotation_.z, rotation_.x, rotation_.y);           // ワールド行列に回転を反映
	matWorld_ *= Matrix4::Translation(translation_.x, translation_.y, translation_.z);          // ワールド行列に平行移動を反映

	Vector3 eyePosition = camera->Geteye();

	Vector3 targetPosition =camera->Gettarget();

	Vector3 upVector = camera->Getup();

	Vector3 cameraAxisZ = targetPosition-eyePosition;

	cameraAxisZ.Normalize();

	Vector3 cameraAxisX;

	cameraAxisX = upVector.Cross(cameraAxisZ);

	cameraAxisX.Normalize();

	Vector3 cameraAxisY;

	cameraAxisY = cameraAxisZ.Cross(cameraAxisX);

	Matrix4 matCameraRot = { Float4(cameraAxisX, 0), Float4(cameraAxisY, 0), Float4(cameraAxisZ, 0), Float4(0,0,0,1) };


	Matrix4 matView = matCameraRot.GetTranspose();

	Vector3 reverseEyePosition = -eyePosition;

	float tX = cameraAxisX.Dot(reverseEyePosition);
	float tY = cameraAxisY.Dot(reverseEyePosition);
	float tZ = cameraAxisZ.Dot(reverseEyePosition);

	Vector3 translation = { tX,tY,tZ };

	matView.SetTranslation(Float4(translation, 1.0f));

	matBillboard = { Float4(cameraAxisX, 0), Float4(cameraAxisY, 0), Float4(cameraAxisZ, 0), Float4(0,0,0,1) };
	
	//// 親行列の指定がある場合は、掛け算する
	//if (parent_) {
	//	matWorld_ *= parent_->matWorld_;
	//}

	HRESULT result = constBuffB0->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));
	(void)result;
	// 定数バッファに書き込み
	constMap->viewproj = matView * camera->GetMatProjection();
	constMap->world = matBillboard;
	constBuffB0->Unmap(0, nullptr);
	

}

void WorldTransform::UpdateSpriteMatrix(Matrix4 projection)
{
	matWorld_ = Matrix4();

	// ワールド行列の合成
	matWorld_ *= Matrix4::Scaling(scale_.x, scale_.y, 1.0f);					// ワールド行列にスケーリングを反映
	matWorld_ *= Matrix4::RotationZXY(rotation_.z, 0, 0);							// ワールド行列に回転を反映
	matWorld_ *= Matrix4::Translation(translation_.x, translation_.y, 0);			// ワールド行列に平行移動を反映


	//// 親行列の指定がある場合は、掛け算する
	//if (parent_) {
	//    matWorld_ *= parent_->matWorld_;
	//}

	HRESULT result = constBuffB0->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));
	(void)result;
	// 定数バッファに書き込み
	constMap->color = color;
	constMap->world = matWorld_ * projection;

}
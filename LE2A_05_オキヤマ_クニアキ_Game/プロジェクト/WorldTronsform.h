#pragma once

#include <DirectXMath.h>
#include <d3d12.h>
#include <wrl.h>

using namespace DirectX;

using namespace Microsoft::WRL;

// �萔�o�b�t�@�p�f�[�^�\����
struct ConstBufferDataWorldTransformB0
{
	XMFLOAT4 color;
	XMMATRIX viewproj;
	XMMATRIX world;
	XMFLOAT3 cameraPos;
};



/// <summary>
/// ���[���h�ϊ��f�[�^
/// </summary>
struct WorldTransform {

	
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffB0;
	// �}�b�s���O�ς݃A�h���X
	ConstBufferDataWorldTransformB0* constMap = nullptr;
	// ���[�J���X�P�[��
	XMFLOAT3 scale_ = { 1, 1, 1 };
	// X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation_ = { 0, 0, 0 };
	// ���[�J�����W
	XMFLOAT3 translation_ = { 0, 0, 0 };

	XMFLOAT4 color = { 1,1,1,1 };
	// ���[�J�� �� ���[���h�ϊ��s��
	XMMATRIX matWorld_;
	// �e�ƂȂ郏�[���h�ϊ��ւ̃|�C���^
	//WorldTransform* parent_ = nullptr;

	/// <summary>
	/// �萔�o�b�t�@����
	/// </summary>
	void CreateConstBuffer(ID3D12Device* device);
	/// <summary>
	/// �}�b�s���O����
	/// </summary>
	void Map();
	/// <summary>
	/// �s����X�V����
	/// </summary>
	void UpdateMatrix(XMMATRIX view,XMMATRIX projection,XMFLOAT3 camerapos);

	void UpdateSpriteMatrix(XMMATRIX projection);
};

#include "DirectionLight.h"
#include<cassert>
using namespace DirectX;

ID3D12Device* DirectionLight::device = nullptr;

void DirectionLight::StaticInitialize(ID3D12Device* device)
{
	assert(!DirectionLight::device);
	assert(device);
	DirectionLight::device = device;


}

DirectionLight* DirectionLight::Create()
{
	DirectionLight* instance = new DirectionLight();

	instance->Initialize();

	return instance;
}

void DirectionLight::Initialize()
{
	HRESULT result;
	D3D12_HEAP_PROPERTIES cbHeapProp{};

	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p

	D3D12_RESOURCE_DESC cbResourceDesc{};
	//���\�[�X�ݒ�
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0Xff;	//256�o�C�g�A���C�����g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&cbHeapProp,		//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
	);
	assert(SUCCEEDED(result));

	TransferConstBuffer();

}

void DirectionLight::TransferConstBuffer()
{
	HRESULT result;

	ConstBufferData* constMap = nullptr;

	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->lightv = -lightdir;
		constMap->lightcolor = lightcolor;
		constBuff->Unmap(0, nullptr);
	}



}

void DirectionLight::Update()
{
	if (dirty)
	{
		TransferConstBuffer();
		dirty = false;
	}
}

void DirectionLight::Draw(ID3D12GraphicsCommandList* cmdlist, UINT rootParamIndex)
{
	cmdlist->SetGraphicsRootConstantBufferView(rootParamIndex, constBuff->GetGPUVirtualAddress());
}

void DirectionLight::SetLightDir(const XMVECTOR& lightdir)
{
	this->lightdir = XMVector3Normalize(lightdir);
	dirty = true;
}

void DirectionLight::SetLightColor(const XMFLOAT3& lightcolor)
{
	this->lightcolor = lightcolor;
	dirty = true;
}

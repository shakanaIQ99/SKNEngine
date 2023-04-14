#include "Sprite2D.h"

void Sprite2D::Initialize(SpriteCommon* spritecommon, WorldTransform* wt, uint32_t handle)
{
	this->spritecommon = spritecommon;
	this->Wt = wt;
	tex = TextureManager::GetTextureData(handle);

	device = spritecommon->GetDirextXCommon()->GetDevice();


	HRESULT result;

	VertexPos vertices[] = {
		{{  0.0f,100.0f,0},{0.0f,1.0f} },
		{{  0.0f,  0.0f,0},{0.0f,0.0f} },
		{{100.0f,100.0f,0},{1.0f,1.0f} },
		{{100.0f,  0.0f,0},{1.0f,0.0f} },

	};

	uint16_t indices[] =
	{
		0,1,2,
		1,2,3,
	};
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);
	assert(SUCCEEDED(result));


	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;

	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;	//�C���f�b�N�X��񂪓���T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = device->CreateCommittedResource(
		&heapProp,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);

	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result))
	{
		memcpy(indexMap, indices, sizeof(indices));
		indexBuff->Unmap(0, nullptr);
	}

	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;


	D3D12_HEAP_PROPERTIES cbHeapProp{};

	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p

	D3D12_RESOURCE_DESC cbResourceDesc{};
	//���\�[�X�ݒ�
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0Xff;	//256�o�C�g�A���C�����g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	this->Wt->CreateConstBuffer(device);

	this->Wt->Map();

	this->Wt->UpdateSpriteMatrix(spritecommon->Getmat());


}

void Sprite2D::Update()
{
	this->Wt->Map();

	this->Wt->UpdateSpriteMatrix(spritecommon->Getmat());
}

void Sprite2D::Draw()
{
	spritecommon->DrawCommand(tex,vbView,ibView,Wt);

}


#include "VertexBuffer.h"

void VertexBuffer::Create(ID3D12Device* device, size_t length, size_t singleSize, const void* data)
{
	// ヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};

	//頂点データ全体のサイズ = 頂点データサイズ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(singleSize * length);

	//頂点バッファの設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用
	//リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// リソースを生成
	HRESULT result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(vertBuff.ReleaseAndGetAddressOf()));

	// 頂点バッファビューの設定
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = static_cast<UINT>(length * singleSize);
	vbView.StrideInBytes = static_cast<UINT>(singleSize);

	result = vertBuff->Map(0, nullptr, &bufferMappedPtr);

	// マッピングする
	if (data != nullptr)
	{
		// 頂点データをマッピング先に設定
		memcpy(bufferMappedPtr, data, length * singleSize);
	}

}

D3D12_VERTEX_BUFFER_VIEW VertexBuffer::GetView() const
{
	return vbView;
}

ID3D12Resource* VertexBuffer::GetResource()
{
	return vertBuff.Get();
}

void VertexBuffer::Update(void* data)
{
	if (data == nullptr)
	{
		return;
	}

	// 頂点データをマッピング先に設定
	memcpy(bufferMappedPtr, data, vbView.SizeInBytes);
}

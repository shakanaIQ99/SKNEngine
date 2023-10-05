#include "IndexBuffer.h"

void IndexBuffer::Create(ID3D12Device* device, size_t length, const void* data)
{
	bufferLength = length;

	//ヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};

	//インデックスデータのサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint32_t) * length);
	//頂点バッファの設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用
	//リソース設定　
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;
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
		IID_PPV_ARGS(indexBuff.ReleaseAndGetAddressOf()));

	// インデックスバッファビューの設定
	ibView = {};
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R32_UINT;
	ibView.SizeInBytes = static_cast<UINT>(length * sizeof(uint32_t));

	result =indexBuff->Map(0, nullptr, &bufferMappedPtr);

	// マッピングする
	if (data != nullptr)
	{
		// インデックスデータをマッピング先に設定
		memcpy(bufferMappedPtr, data, length * sizeof(uint32_t));

		// マッピング解除
		indexBuff->Unmap(0, nullptr);
	}

}

D3D12_INDEX_BUFFER_VIEW IndexBuffer::GetView() const
{
	return ibView;
}

void IndexBuffer::Update(void* data)
{
	memcpy(bufferMappedPtr, data, bufferLength * sizeof(uint32_t));
}

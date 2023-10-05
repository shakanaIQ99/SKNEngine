#include "ConstBuffer.h"

void ConstBuffer::Create(DirectXCommon* dxcommon, size_t size)
{
	bufferSize = size;

	//頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};//ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用
	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = (bufferSize + 0xff) & ~0xff;//頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// リソースを生成
	HRESULT result = dxcommon->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(buffer.ReleaseAndGetAddressOf()));

	result = buffer->Map(0, nullptr, &bufferMappedPtr);

	constantBufferView = {};
	constantBufferView.BufferLocation = buffer->GetGPUVirtualAddress();
	constantBufferView.SizeInBytes = static_cast<UINT>(resDesc.Width);

	dxcommon->GetDescriptorHeap()->CreateCBV(constantBufferView);


}

D3D12_GPU_VIRTUAL_ADDRESS ConstBuffer::GetAddress() const
{
	return D3D12_GPU_VIRTUAL_ADDRESS();
}

D3D12_CONSTANT_BUFFER_VIEW_DESC ConstBuffer::GetViewDesc()
{
	return D3D12_CONSTANT_BUFFER_VIEW_DESC();
}

//void ConstBuffer::Update(void* data)
//{
//}

ID3D12Resource* ConstBuffer::GetResource()
{
	return nullptr;
}

void* ConstBuffer::GetPtr()
{
	return nullptr;
}

#include "TextureManager.h"
TextureManager* TextureManager::texManager = nullptr;
vector<string>TextureManager::FilePaths;
unordered_map<string, unique_ptr<TextureData>> TextureManager::texDatas;



void TextureManager::StaticInitialize(DirectXCommon* dxcommon)
{
	dxCommon = dxcommon;

	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	FilePaths.resize(2056);
}

uint32_t TextureManager::LoadTexture(const string& path)
{
	if (TextureSize > 2056)
	{
		assert(0);
	}

	auto itreter = find_if(texDatas.begin(), texDatas.end(), [&](pair<const string, unique_ptr<TextureData, default_delete<TextureData>>>& ptr)
		{
			return ptr.second->path == path;
		});

	if (itreter == texDatas.end())
	{
		unique_ptr<TextureData> data;

		data.reset(LoadFromTextureData(path));
		data->texHandle = TextureSize;
		data->path = path;

		texDatas[path] = move(data);
		FilePaths[TextureSize] = path;
		uint32_t handl = TextureSize;
		TextureSize++;


		return handl;
	}
	else
	{

		uint32_t modelHandle = texDatas[path]->texHandle;

		return modelHandle;
	}
}

uint32_t TextureManager::Load(const string& path)
{
	return TextureManager::GetInstance()->LoadTexture(path);
}

TextureData* TextureManager::GetTextureData(uint32_t handle)
{
	return texDatas[FilePaths[handle]].get();
}

TextureManager* TextureManager::GetInstance()
{
	if (!texManager)
	{
		texManager = new TextureManager();
	}

	return texManager;
}

void TextureManager::DeleteInstance()
{
	delete texManager;
}

void TextureManager::FileLoad(const string& path, TexMetadata& metadata, ScratchImage& scratchImg)
{
	wchar_t Filepath[256];
	HRESULT result;

	MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, Filepath, _countof(Filepath));
	
	result = LoadFromWICFile(
		Filepath,
		DirectX::WIC_FLAGS_NONE,
		&metadata, scratchImg);
	assert(SUCCEEDED(result));


}

TextureData* TextureManager::LoadFromTextureData(const string& path)
{
	TextureData* texdata = new TextureData();
	HRESULT result;

	TexMetadata metadata{};
	ScratchImage scratchImg{};
	ScratchImage mipChain{};

	texdata->srvHeap = dxCommon->GetDescriptorHeap()->GetHeap();

	FileLoad(path, metadata, scratchImg);

	
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format =MakeSRGB(metadata.format);

	texdata->texBuff = CreateTexBuff(metadata, scratchImg);

	texdata->gpuHandle = CreateSRV(texdata->texBuff.Get(), metadata);

	texdata->width = metadata.width;
	texdata->height = metadata.height;

	return texdata;
}

ComPtr<ID3D12Resource> TextureManager::CreateTexBuff(TexMetadata& metadata, ScratchImage& scratchImg)
{
	ComPtr<ID3D12Resource> texbuff;
	HRESULT result;

	D3D12_RESOURCE_DESC rsDesc{};
	rsDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	rsDesc.Format = metadata.format;
	rsDesc.Width = metadata.width;
	rsDesc.Height = (UINT)metadata.height;
	rsDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	rsDesc.MipLevels = (UINT16)metadata.mipLevels;
	rsDesc.SampleDesc.Count = 1;

	result = dxCommon->GetDevice()->CreateCommittedResource(
		&texHeapProp,		//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&rsDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(texbuff.ReleaseAndGetAddressOf())
	);

	for (size_t i = 0; i < metadata.mipLevels; i++) {
		const Image* img = scratchImg.GetImage(i, 0, 0); // 生データ抽出
		result = texbuff->WriteToSubresource(
			(UINT)i,
			nullptr,              // 全領域へコピー
			img->pixels,          // 元データアドレス
			(UINT)img->rowPitch,  // 1ラインサイズ
			(UINT)img->slicePitch // 1枚サイズ
		);
		assert(SUCCEEDED(result));
	}

	return texbuff;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::CreateSRV(ID3D12Resource* texBuff, TexMetadata& metadata)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = static_cast<UINT>(metadata.mipLevels);

	D3D12_GPU_DESCRIPTOR_HANDLE srvgpudesc;

	srvgpudesc.ptr = dxCommon->GetDescriptorHeap()->CreateSRV(srvDesc, texBuff);

	return srvgpudesc;
}



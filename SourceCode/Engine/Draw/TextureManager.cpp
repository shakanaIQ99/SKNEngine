#include "TextureManager.h"

using namespace SKNEngine;

TextureHandle TextureManager::LoadTexture(const string& FilePath, TextureHandle Handle)
{
	std::unique_lock<std::recursive_mutex> lock(mutex);
	HRESULT result;

	//一回読み込んだことがあるファイルはそのまま返す
	auto itr = find_if(textureMap.begin(), textureMap.end(), [&](const std::pair<TextureHandle, TextureData>& p) {
		return p.second.filePath == FilePath;
		});
	if (itr != textureMap.end()) {
		return itr->first;
	}
	lock.unlock();


	TextureData texdata = TextureData();

	TexMetadata metadata{};
	ScratchImage scratchImg{};

	wchar_t wFilePath[256];

	MultiByteToWideChar(CP_ACP, 0, FilePath.c_str(), -1, wFilePath, _countof(wFilePath));

	result = LoadFromWICFile(
		wFilePath,
		DirectX::WIC_FLAGS_NONE,
		&metadata, scratchImg);
	if (FAILED(result)) {
		return "FailedTextureHandle";
	}

	ScratchImage mipChain{};

	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}
	else 
	{
		return "FailedTextureHandle";
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

	D3D12_HEAP_PROPERTIES texHeapProp{};
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	D3D12_RESOURCE_DESC rsDesc{};
	rsDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	rsDesc.Format = metadata.format;
	rsDesc.Width = metadata.width;
	rsDesc.Height = (UINT)metadata.height;
	rsDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	rsDesc.MipLevels = (UINT16)metadata.mipLevels;
	rsDesc.SampleDesc.Count = 1;

	result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&texHeapProp,		//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&rsDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texdata.texResource)
	);
	if (FAILED(result)) {
		return "FailedTextureHandle";
	}

	for (size_t i = 0; i < metadata.mipLevels; i++) {
		const Image* img = scratchImg.GetImage(i, 0, 0); // 生データ抽出
		result = texdata.texResource->WriteToSubresource(
			(UINT)i,
			nullptr,              // 全領域へコピー
			img->pixels,          // 元データアドレス
			(UINT)img->rowPitch,  // 1ラインサイズ
			(UINT)img->slicePitch // 1枚サイズ
		);
		if (FAILED(result)) {
			return "FailedTextureHandle";
		}
	}

	return CreateHandle(texdata, Handle);
	
}

TextureHandle TextureManager::CreateHandle(TextureData TexData, TextureHandle Handle)
{

	std::unique_lock<std::recursive_mutex> lock(mutex);
	int useIndex = -1;

	auto itr = textureMap.find(Handle);
	if (itr != textureMap.end()) {
		useIndex = itr->second.Index;
	}
	else {
		for (UINT i = 0; i < 2056; i++) {
			bool ok = true;
			for (std::pair<const TextureHandle, TextureData>& p : textureMap) {
				if (p.second.Index == static_cast<int>(i)) {
					ok = false;
					break;
				}
			}

			if (ok) {
				useIndex = i;
				break;
			}
		}
	}
	lock.unlock();

	if (useIndex == -1) 
	{
		return TextureHandle();
	}


	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = TexData.texResource->GetDesc().Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = TexData.texResource->GetDesc().MipLevels;

	TexData.gpuHandle.ptr = DirectXCommon::GetInstance()->GetDescriptorHeap()->CreateSRV(srvDesc, TexData.texResource.Get());
	TexData.Index = useIndex;

	if (Handle.empty()) {
		Handle = "NoNameHandle_" + to_string(useIndex);
	}

	lock.lock();
	textureMap[Handle] = TexData;
	return Handle;
}

TextureData& TextureManager::Get(const TextureHandle& Handle)
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	if (Handle.empty()) {
		return textureMap["EMPTYHANDOLE"];
	}

	auto itr = textureMap.find(Handle);
	if (itr != textureMap.end()) {
		return itr->second;
	}

	return textureMap["ERRORTEXTURE"];
}


TextureManager* TextureManager::GetInstance()
{
	
	static TextureManager instance;
	return &instance;
	
}

TextureHandle TextureManager::Load(const std::string FilePath, const std::string Handle)
{
	TextureManager* manager = TextureManager::GetInstance();
	return manager->LoadTexture(FilePath,Handle);
}

TextureData& TextureManager::GetTextureData(const TextureHandle& Handle)
{
	TextureManager* manager = TextureManager::GetInstance();
	return manager->Get(Handle);
}

void TextureManager::StaticFinalize()
{
	TextureManager* manager = TextureManager::GetInstance();
	manager->textureMap.clear();
}


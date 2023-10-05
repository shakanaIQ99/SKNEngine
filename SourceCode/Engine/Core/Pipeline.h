#pragma once
#include <d3d12.h>
#include<wrl.h>
#include<memory>
#include<array>
//#include <cstdint>

using namespace Microsoft::WRL;


struct PipelineSet
{
	//ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature;
	//パイプランステートの生成
	ComPtr<ID3D12PipelineState> pipelineState;
};

class Pipeline
{
public:

	static PipelineSet CreateSpritePipline(ID3D12Device* device);
	static PipelineSet CreateModelPipline(ID3D12Device* device);
	static PipelineSet CreateParticlePipline(ID3D12Device* device);
	static PipelineSet CreateFBXPipeline(ID3D12Device* device);
	static PipelineSet CreatePostEffectPipeline(ID3D12Device* device);
	static PipelineSet Create3DLinePipeline(ID3D12Device* device);

};


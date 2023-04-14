#pragma once
#include <d3d12.h>
#include<wrl.h>
#include<memory>
#include<array>
//#include <cstdint>

using namespace Microsoft::WRL;


struct PipelineSet
{
	//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature;
	//�p�C�v�����X�e�[�g�̐���
	ComPtr<ID3D12PipelineState> pipelineState;
};

class Pipeline
{
public:

	static PipelineSet CreateSpritePipline(ID3D12Device* device);
	static PipelineSet CreateModelPipline(ID3D12Device* device);


};


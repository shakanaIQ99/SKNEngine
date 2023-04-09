#include "ParticleManager.h"
ID3D12Device* ParticleManager::device = nullptr;
ID3D12GraphicsCommandList* ParticleManager::commandList;

void ParticleManager::StaticInitialize(ID3D12Device* device, int window_width, int window_height)
{
	assert(device);
	ParticleManager::device = device;
	ParPipeline = Pipeline::CreateParticlePipline(device);

}

void ParticleManager::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	commandList = cmdList;

	commandList->SetPipelineState(ParPipeline.pipelineState.Get());
	commandList->SetGraphicsRootSignature(ParPipeline.rootSignature.Get());
	// プリミティブ形状の設定コマンド
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト
}

void ParticleManager::PostDraw()
{
	ParticleManager::commandList = nullptr;
}

ParticleManager* ParticleManager::Create(uint32_t Handle)
{
	return nullptr;
}

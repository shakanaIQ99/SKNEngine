#include "ParticleManager.h"
#include"DirectXCommon.h"
PipelineSet ParticleManager::parPipeline;
ComPtr<ID3D12Resource> ParticleManager::vertBuff;
D3D12_VERTEX_BUFFER_VIEW ParticleManager::vbView{};
ParticleManager::VertexPos ParticleManager::vertices[vertexCount];

using namespace SKNEngine;


ParticleManager::ParticleManager(TextureHandle Handle)
{
	handle = Handle;
}

void ParticleManager::StaticInitialize()
{
	parPipeline = Pipeline::CreateParticlePipline(DirectXCommon::GetInstance()->GetDevice().Get());
	CreateModel();

}

void ParticleManager::PreDraw()
{
	
	DirectXCommon::GetInstance()->GetCommandList()->SetPipelineState(parPipeline.pipelineState.Get());
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootSignature(parPipeline.rootSignature.Get());
	// プリミティブ形状の設定コマンド
	DirectXCommon::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST); // 三角形リスト
}


void ParticleManager::CreateModel()
{
	HRESULT result = S_FALSE;

	vector<VertexPos> realVertices;


	unsigned short indicesSquare[] = {
		0,1,2,
		2,1,3,
	};

	//std::copy(std::begin(indicesSquare), std::end(indicesSquare), indices);

	UINT sizeVB = static_cast<UINT>(sizeof(vertices));

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

	result = DirectXCommon::GetInstance()->GetDevice().Get()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);
	assert(SUCCEEDED(result));


	// 頂点バッファへのデータ転送
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		memcpy(vertMap, vertices, sizeof(vertices));
		//memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);
}

ParticleManager* ParticleManager::Create(TextureHandle Handle)
{
	// Spriteのインスタンスを生成
	ParticleManager* pat = new ParticleManager(Handle);

	if (pat == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!pat->Initialize()) {
		delete pat;
		assert(0);
		return nullptr;
	}

	return pat;
}

bool ParticleManager::Initialize()
{
	wt.CreateConstBuffer();
	
	return true;
}

void ParticleManager::Update(ViewProjection* camera)
{

	HRESULT result;

	particle.remove_if([](Particle& x) {return x.frame >= x.num_frame; });

	for (std::forward_list<Particle>::iterator it = particle.begin(); it != particle.end(); it++)
	{
		it->frame++;

		it->velocity = it->velocity + it->accel;

		it->position = it->position + it->velocity;

		float f = (float)it->frame / it->num_frame;

		it->scale = (it->e_scale - it->s_scale) * f;
		it->scale += it->s_scale;
	}



	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		for (std::forward_list<Particle>::iterator it = particle.begin(); it != particle.end(); it++)
		{
			vertMap->pos = it->position;

			vertMap->scale = it->scale;

			vertMap++;
		}
		vertBuff->Unmap(0, nullptr);
	}



	wt.UpdateMatrixBill(camera);
}

void ParticleManager::Draw()
{
	DirectXCommon::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

	DirectXCommon::GetInstance()->GetCommandList()->SetDescriptorHeaps(1,SKNEngine::DirectXCommon::GetInstance()->GetDescriptorHeap()->GetHeap().GetAddressOf());
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = TextureManager::GetTextureData(handle).gpuHandle;

	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, wt.constBuffB0->GetGPUVirtualAddress());

	DirectXCommon::GetInstance()->GetCommandList()->DrawInstanced((UINT)std::distance(particle.begin(), particle.end()), 1, 0, 0);

}

void ParticleManager::Add(int life, Vector3 position, Vector3 velocity, Vector3 accel, float start_scale, float end_scale)
{
	particle.emplace_front();

	Particle& p = particle.front();

	p.position = position;
	p.velocity = velocity;
	p.accel = accel;
	p.num_frame = life;
	p.s_scale = start_scale;
	p.e_scale = end_scale;
}



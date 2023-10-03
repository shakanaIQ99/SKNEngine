#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include"WorldTronsform.h"
#include<vector>
#include<string>
#include"Pipeline.h"
#include"ViewProjection.h"
#include<forward_list>
#include"TextureManager.h"
#include<Camera.h>

using namespace DirectX;

using namespace Microsoft::WRL;

using namespace std;

class ParticleManager
{
public:

	ParticleManager(uint32_t handle);
	struct VertexPos
	{
		XMFLOAT3 pos; // xyz座標
		float scale;
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMMATRIX mat;	// ３Ｄ変換行列
		XMMATRIX matBillboard;
	};

	struct Particle
	{
		using XMFLOAT3 = DirectX::XMFLOAT3;

		XMFLOAT3 position = {};

		XMFLOAT3 velocity = {};

		XMFLOAT3 accel = {};

		int frame = 0;

		int num_frame = 0;

		float scale = 1.0f;
		float s_scale = 1.0f;
		float e_scale = 0.0f;
	};

	std::forward_list<Particle> particle;

	static void StaticInitialize(ID3D12Device* device);

	static void PreDraw(ID3D12GraphicsCommandList* cmdList);
	
	static void PostDraw();

	static void CreateModel();

	static ParticleManager* Create(uint32_t Handle);

	bool Initialize();

	void Update(ViewProjection* camera);

	void Draw();

	void Add(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel, float start_scale, float end_scale);

private:
	static ID3D12Device* device;

	static ID3D12GraphicsCommandList* commandList;

	static PipelineSet ParPipeline;

	static const int vertexCount = 1024;

	static VertexPos vertices[vertexCount];

	static ComPtr<ID3D12Resource> vertBuff;
	static D3D12_VERTEX_BUFFER_VIEW vbView;

	TextureData* tex;

	WorldTransform wt;
	
};

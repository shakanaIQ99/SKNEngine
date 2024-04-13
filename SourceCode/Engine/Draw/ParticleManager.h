#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
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

	ParticleManager(TextureHandle Handle);
	struct VertexPos
	{
		Vector3 pos; // xyz座標
		float scale;
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		Matrix4 mat;	// ３Ｄ変換行列
		Matrix4 matBillboard;
	};

	struct Particle
	{
		
		Vector3 position = {};

		Vector3 velocity = {};

		Vector3 accel = {};

		int frame = 0;

		int num_frame = 0;

		float scale = 1.0f;
		float s_scale = 1.0f;
		float e_scale = 0.0f;
	};

	std::forward_list<Particle> particle;

	static void StaticInitialize();

	static void PreDraw();
	


	static void CreateModel();

	static ParticleManager* Create(TextureHandle Handle);

	bool Initialize();

	void Update(ViewProjection* camera);

	void Draw();

	void Add(int life, Vector3 position, Vector3 velocity, Vector3 accel, float start_scale, float end_scale);

private:

	static PipelineSet parPipeline;

	static const int vertexCount = 1024;

	static VertexPos vertices[vertexCount];

	static ComPtr<ID3D12Resource> vertBuff;
	static D3D12_VERTEX_BUFFER_VIEW vbView;

	TextureHandle handle;

	WorldTransform wt;
	
};

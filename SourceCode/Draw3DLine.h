#pragma once
#include "Pipeline.h"
#include "VertexBuffer.h"
#include "Camera.h"
#include "WorldTronsform.h"
#include"Pipeline.h"
#include <wrl.h>
class Draw3DLine
{
public:

	void Init();

	void Draw(XMFLOAT3 startpos, XMFLOAT3 endpos);

	void SetColor(XMFLOAT4 _color)
	{
		wt.color = _color;
	}

	static void SetDevice(ID3D12Device* _device, ID3D12GraphicsCommandList* _cmdList)
	{
		device = _device;
		cmdList = _cmdList;
		CreateGraphicsPipeline();
	}
	static void CreateGraphicsPipeline()
	{
		pipeline = Pipeline::Create3DLinePipeline(device);
	}

	static void SetCamera(Camera* _camera)
	{
		camera = _camera;
	}

private:

	struct VertexPos
	{
		XMFLOAT3 pos; // xyzç¿ïW
	};

	static ID3D12Device* device;
	static ID3D12GraphicsCommandList* cmdList;
	static PipelineSet pipeline;
	static Camera* camera;

	WorldTransform wt;

	std::unique_ptr<VertexBuffer> vertexBuffer = {};
	D3D12_VERTEX_BUFFER_VIEW vbView;

	XMFLOAT4 color = { 1.0f,0.0f, 1.0f, 1.0f };

};


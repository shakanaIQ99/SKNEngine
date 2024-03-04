#pragma once
#include "Pipeline.h"
#include "VertexBuffer.h"
#include "Camera.h"
#include "WorldTronsform.h"
#include"Pipeline.h"
#include <wrl.h>
#include"myMath.h"
class Draw3DLine
{
public:

	void Init();

	void Draw(Vector3 startpos, Vector3 endpos);

	void SetColor(Float4 _color){color = _color;}

	static void SetDevice(ID3D12Device* _device, ID3D12GraphicsCommandList* _cmdList);

	static void CreateGraphicsPipeline(){pipeline = Pipeline::Create3DLinePipeline(device);}

	static void SetCamera(Camera* _camera){camera = _camera;}


private:
	void CreateColorBuff();

	void TransferColor();

	struct VertexPos
	{
		Vector3 pos; // xyz座標
	};

	struct Color
	{
		Float4 color;
	};

	static ID3D12Device* device;
	static ID3D12GraphicsCommandList* cmdList;
	static PipelineSet pipeline;
	static Camera* camera;

	WorldTransform wt;

	std::unique_ptr<VertexBuffer> vertexBuffer = {};
	D3D12_VERTEX_BUFFER_VIEW vbView;

	ComPtr<ID3D12Resource> constBuffB1;
	Color* constMap = nullptr;

	Float4 color = { 1.0f,1.0f,1.0f,1.0f };

};


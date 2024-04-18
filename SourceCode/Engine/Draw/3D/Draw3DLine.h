#pragma once
#include "Pipeline.h"
#include "VertexBuffer.h"
#include "Camera.h"
#include "WorldTronsform.h"
#include"Pipeline.h"
#include <wrl.h>
#include"myMath.h"
#include"DirectXCommon.h"
class Draw3DLine
{
public:

	void Initialize();

	void Draw(Vector3 startpos, Vector3 endpos);

	void SetColor(Float4 _color){color = _color;}


	static void CreateGraphicsPipeline(){pipeline = Pipeline::Create3DLinePipeline(SKNEngine::DirectXCommon::GetDevice().Get());}

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

	static PipelineSet pipeline;
	static Camera* camera;

	WorldTransform wt;

	std::unique_ptr<VertexBuffer> vertexBuffer = {};
	D3D12_VERTEX_BUFFER_VIEW vbView;

	ComPtr<ID3D12Resource> constBuffB1;
	Color* constMap = nullptr;

	Float4 color = { 1.0f,1.0f,1.0f,1.0f };

};


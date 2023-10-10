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

	void SetColor(XMFLOAT4 _color){color = _color;}

	static void CreateGraphicsPipeline();

	static void SetCamera(Camera* _camera){camera = _camera;}


private:
	void CreateColorBuff();

	void TransferColor();

	struct VertexPos
	{
		XMFLOAT3 pos; // xyz座標
	};

	struct Color
	{
		XMFLOAT4 color;
	};

	static PipelineSet pipeline;
	static Camera* camera;

	WorldTransform wt;

	std::unique_ptr<VertexBuffer> vertexBuffer = {};
	D3D12_VERTEX_BUFFER_VIEW vbView;

	ComPtr<ID3D12Resource> constBuffB1;
	Color* constMap = nullptr;

	XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };

};


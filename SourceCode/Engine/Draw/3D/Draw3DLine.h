#pragma once
//3D線
#include "Pipeline.h"
#include "VertexBuffer.h"
#include "Camera.h"
#include "WorldTronsform.h"
#include"Pipeline.h"
#include <wrl.h>

class Draw3DLine
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="startpos"></param>
	/// <param name="endpos"></param>
	void Draw(XMFLOAT3 startpos, XMFLOAT3 endpos);

	/// <summary>
	/// 色設定
	/// </summary>
	/// <param name="_color"></param>
	void SetColor(XMFLOAT4 _color){color = _color;}

	/// <summary>
	/// パイプライン生成
	/// </summary>
	static void CreateGraphicsPipeline();

	/// <summary>
	/// カメラ設定
	/// </summary>
	/// <param name="_camera"></param>
	static void SetCamera(Camera* _camera){camera = _camera;}


private:
	/// <summary>
	/// 色バッファ
	/// </summary>
	void CreateColorBuff();

	/// <summary>
	/// 色転送
	/// </summary>
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


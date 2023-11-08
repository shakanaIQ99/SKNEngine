#pragma once
//後々Modelを改造しクラス統合予定
#include<string>
#include"DxWindow.h"
#include<vector>
#include<DirectXMath.h>
#include <d3d12.h>
#include <wrl.h>
#include"TextureManager.h"
#include"WorldTronsform.h"
#include"IndexBuffer.h"
#include"VertexBuffer.h"


using namespace DirectX;

using namespace Microsoft::WRL;

using namespace std;

class Modeldata
{
public:


	struct VertexPos
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal;
		XMFLOAT2 uv;
	};


	struct Material
	{
		string name;
		XMFLOAT3 ambient;
		XMFLOAT3 diffuse;
		XMFLOAT3 specular;

		float alpha;
		string textureFilename;

		Material()
		{
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;

		}

	};

	struct ConstBufferDataMaterial
	{
		XMFLOAT3 ambient;
		float pad1;
		XMFLOAT3 diffuse;
		float pad2;
		XMFLOAT3 specular;
		float alpha;
	};


	Material material;


private:



};


class ModelManager
{
public:


private:

};


class ObjModel
{

public:

	struct VertexPos
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal;
		XMFLOAT2 uv;
	};


	struct Material
	{
		string name;
		XMFLOAT3 ambient;
		XMFLOAT3 diffuse;
		XMFLOAT3 specular;

		float alpha;
		string textureFilename;

		Material()
		{
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;

		}

	};

	struct ConstBufferDataMaterial
	{
		XMFLOAT3 ambient;
		float pad1;
		XMFLOAT3 diffuse;
		float pad2;
		XMFLOAT3 specular;
		float alpha;
	};


	Material material;

	/// <summary>
	/// 読み込んだモデルを取得
	/// </summary>
	/// <param name="modelname"></param>
	/// <param name="smoothing"></param>
	/// <returns></returns>
	static ObjModel* LoadFromOBJ(const string& modelname, bool smoothing = false);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="rootParamIndexMaterial"></param>
	void Draw(UINT rootParamIndexMaterial);

private:

	/// <summary>
	/// モデル読み込み
	/// </summary>
	/// <param name="modelname"></param>
	/// <param name="smoothing"></param>
	void LoadFromOBJInternal(const string& modelname,bool smoothing);

	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="filename"></param>
	void LoadMaterial(const string& directoryPath, const string& filename);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="filename"></param>
	void LoadTexture(const string& directoryPath, const string& filename);

	/// <summary>
	/// バッファ生成
	/// </summary>
	void CreateBuffers();

	/// <summary>
	/// スムージング処理
	/// </summary>
	/// <param name="indexPosition"></param>
	/// <param name="indexVertex"></param>
	void AddSmoothData(unsigned short indexPosition, unsigned short indexVertex);

	/// <summary>
	/// スムージング計算
	/// </summary>
	void CaliculateSmoothedVertexNormals();

	vector<unsigned short>indices;
	vector<VertexPos> vertices;

	ComPtr<ID3D12Resource> vertBuff;

	ComPtr<ID3D12Resource> indexBuff;

	ComPtr<ID3D12Resource> texBuff;

	D3D12_VERTEX_BUFFER_VIEW vbView;
	D3D12_INDEX_BUFFER_VIEW ibView;

	ComPtr<ID3D12Resource> constBuffB1;

	unordered_map<unsigned short, vector<unsigned short>>smoothData;

	TextureData* tex;
};


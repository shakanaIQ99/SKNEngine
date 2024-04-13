#pragma once
//後々Modelを改造しクラス統合予定
#include<string>
#include"DxWindow.h"
#include<vector>
#include <d3d12.h>
#include <wrl.h>
#include"TextureManager.h"
#include"WorldTronsform.h"
#include"IndexBuffer.h"
#include"VertexBuffer.h"
#include"Vector2.h"


using namespace Microsoft::WRL;

using namespace std;


class ObjModel
{

public:

	struct VertexPos
	{
		Vector3 pos; // xyz座標
		Vector3 normal;
		Vector2 uv;
	};


	struct Material
	{
		string name;
		Vector3 ambient;
		Vector3 diffuse;
		Vector3 specular;

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
		Vector3 ambient;
		float pad1;
		Vector3 diffuse;
		float pad2;
		Vector3 specular;
		float alpha;
	};


	Material material;

	static ObjModel* LoadFromOBJ(const string& modelname, bool smoothing = false);

	static void SetDevice(ID3D12Device* _device) { ObjModel::device = _device; }

	void Draw(ID3D12GraphicsCommandList* commandList, UINT rootParamIndexMaterial);

private:

	static ID3D12Device* device;

	void LoadFromOBJInternal(const string& modelname,bool smoothing);

	void LoadMaterial(const string& directoryPath, const string& filename);

	void LoadTexture(const string& directoryPath, const string& filename);

	void CreateBuffers();

	void AddSmoothData(unsigned short indexPosition, unsigned short indexVertex);

	void CaliculateSmoothedVertexNormals();

	vector<unsigned short>indices;
	vector<VertexPos> vertices;

	ComPtr<ID3D12Resource> vertBuff;

	ComPtr<ID3D12Resource> indexBuff;

	ComPtr<ID3D12Resource> texResource;

	D3D12_VERTEX_BUFFER_VIEW vbView;
	D3D12_INDEX_BUFFER_VIEW ibView;

	ComPtr<ID3D12Resource> constBuffB1;

	unordered_map<unsigned short, vector<unsigned short>>smoothData;

	TextureHandle handle;
};


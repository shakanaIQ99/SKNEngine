#pragma once
//モデル
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
#include <fbxsdk.h>


using namespace DirectX;

using namespace Microsoft::WRL;

using namespace std;

struct Node
{
	string name;

	XMVECTORF32 scaling ;

	XMVECTORF32 rotation ;

	XMVECTORF32 translation ;

	XMMATRIX transform;

	XMMATRIX globalTransform;

	Node* parent = nullptr;

	Node()
	{
		scaling.v = { 1.0f,1.0f,1.0f,0 };
		rotation.v = { 0,0,0,0 };
		translation.v = { 0,0,0,1.0f };
	}
};



class Model
{


private:

	

public:

	~Model();

	static const int MAX_BONE_INDICES = 4;
	//頂点データ構造体
	struct VertexPosNormalUvSkin
	{
		XMFLOAT3 pos;	//xyz座標
		XMFLOAT3 normal;//法線ベクトル
		XMFLOAT2 uv;	//uv座標
		UINT boneIndex[MAX_BONE_INDICES];
		float boneWeight[MAX_BONE_INDICES];
	};
	struct Material
	{
		XMFLOAT3 ambient;
		XMFLOAT3 diffuse;
		XMFLOAT3 specular;

		float alpha;
		Material()
		{
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;

		}

	};
	struct Bone
	{
		std::string name;

		DirectX::XMMATRIX invInitialPose;

		FbxCluster* fbxCluster;

		Bone(const std::string& name)
		{
			this->name = name;
		}
	};

	
	

	friend class FbxLoader;
	FbxScene* fbxScene = nullptr;
	
	
	/// <summary>
	/// バッファ生成
	/// </summary>
	void CreateBuffers();

	/// <summary>
	/// モデルトランスフォーム取得
	/// </summary>
	/// <returns></returns>
	const XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList"></param>
	void Draw();

	/// <summary>
	/// ボーン取得
	/// </summary>
	/// <returns></returns>
	vector<Bone>& GetBones() { return bones; }

	/// <summary>
	/// FBXシーン取得
	/// </summary>
	/// <returns></returns>
	FbxScene* GetFbxScene() { return fbxScene; }
private:

	string name;

	vector<Node> nodes;

	vector<Bone> bones;



	D3D12_VERTEX_BUFFER_VIEW vbView;
	D3D12_INDEX_BUFFER_VIEW ibView;

	//メッシュを持つノード
	Node* meshNode = nullptr;
	//頂点データ
	vector<VertexPosNormalUvSkin>vertices;

	Material material;
	//頂点インデックス配列
	vector<uint32_t> indices;

	//ComPtr<ID3D12Resource> texBuff;

	unique_ptr<VertexBuffer> vertexBuffer = {};

	unique_ptr<IndexBuffer> indexBuffer = {};

	TextureData* tex;

	

};




/*----------------------------以下OBJ------------------------------------------ -
class Model
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
			ambient = { 0.06f,0.06f,0.06f };
			diffuse = { 0.8f,0.8f,0.8f };
			specular = { 0.1f,0.1f,0.1f };
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

	static Model* LoadFromOBJ(const string& modelname, bool smoothing = false);

	static void SetDevice(ID3D12Device* device) { Model::device = device; }

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

	ComPtr<ID3D12Resource> texBuff;

	D3D12_VERTEX_BUFFER_VIEW vbView;
	D3D12_INDEX_BUFFER_VIEW ibView;
	
	ComPtr<ID3D12Resource> constBuffB1;

	unordered_map<unsigned short, vector<unsigned short>>smoothData;

	TextureData* tex;
};

*/
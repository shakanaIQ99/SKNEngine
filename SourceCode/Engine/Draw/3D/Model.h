//#pragma once
//#include<string>
//#include"DxWindow.h"
//#include<vector>
//#include <d3d12.h>
//#include <wrl.h>
//#include"TextureManager.h"
//#include"WorldTronsform.h"
//#include"IndexBuffer.h"
//#include"VertexBuffer.h"
//#include <fbxsdk.h>
//#include"myMath.h"
//
//
//using namespace DirectX;
//
//using namespace Microsoft::WRL;
//
//using namespace std;
//
//struct Node
//{
//	string name;
//
//	Vector3 scaling = { 1,1,1 };
//
//	Vector3 rotation = { 0,0,0 };
//
//	Vector3 translation = { 0,0,0 };
//
//	Matrix4 transform;
//
//	Matrix4 globalTransform;
//
//	Node* parent = nullptr;
//};
//
//
//
//class Model
//{
//
//
//private:
//
//	
//
//public:
//
//	~Model();
//
//	static const int MAX_BONE_INDICES = 4;
//	//頂点データ構造体
//	struct VertexPosNormalUvSkin
//	{
//		Vector3 pos;	//xyz座標
//		Vector3 normal;//法線ベクトル
//		Vector2 uv;	//uv座標
//		UINT boneIndex[MAX_BONE_INDICES];
//		float boneWeight[MAX_BONE_INDICES];
//	};
//	struct Material
//	{
//		Vector3 ambient;
//		Vector3 diffuse;
//		Vector3 specular;
//
//		float alpha;
//		Material()
//		{
//			ambient = { 0.3f,0.3f,0.3f };
//			diffuse = { 0.0f,0.0f,0.0f };
//			specular = { 0.0f,0.0f,0.0f };
//			alpha = 1.0f;
//
//		}
//
//	};
//	struct Bone
//	{
//		std::string name;
//
//		Matrix4 invInitialPose;
//
//		FbxCluster* fbxCluster;
//
//		Bone(const std::string& name)
//		{
//			this->name = name;
//		}
//	};
//
//	
//	
//
//	friend class FbxLoader;
//	FbxScene* fbxScene = nullptr;
//	
//	
//
//	void CreateBuffers();
//
//
//	const Matrix4& GetModelTransform() { return meshNode->globalTransform; }
//	void Draw(ID3D12GraphicsCommandList* cmdList);
//	vector<Bone>& GetBones() { return bones; }
//	FbxScene* GetFbxScene() { return fbxScene; }
//private:
//
//	string name;
//
//	vector<Node> nodes;
//
//	vector<Bone> bones;
//
//
//
//	D3D12_VERTEX_BUFFER_VIEW vbView;
//	D3D12_INDEX_BUFFER_VIEW ibView;
//
//	//メッシュを持つノード
//	Node* meshNode = nullptr;
//	//頂点データ
//	vector<VertexPosNormalUvSkin>vertices;
//
//	Material material;
//	//頂点インデックス配列
//	vector<uint32_t> indices;
//
//	//ComPtr<ID3D12Resource> texBuff;
//
//	unique_ptr<VertexBuffer> vertexBuffer = {};
//
//	unique_ptr<IndexBuffer> indexBuffer = {};
//
//	TextureData* tex;
//
//	
//
//};
//
//
//
//
///*----------------------------以下OBJ------------------------------------------ -
//class Model
//{
//
//public:
//
//	struct VertexPos
//	{
//		XMFLOAT3 pos; // xyz座標
//		XMFLOAT3 normal;
//		XMFLOAT2 uv;
//	};
//
//
//	struct Material
//	{
//		string name;
//		XMFLOAT3 ambient;
//		XMFLOAT3 diffuse;
//		XMFLOAT3 specular;
//
//		float alpha;
//		string textureFilename;
//
//		Material()
//		{
//			ambient = { 0.06f,0.06f,0.06f };
//			diffuse = { 0.8f,0.8f,0.8f };
//			specular = { 0.1f,0.1f,0.1f };
//			alpha = 1.0f;
//
//		}
//
//	};
//
//	struct ConstBufferDataMaterial
//	{
//		XMFLOAT3 ambient;
//		float pad1;
//		XMFLOAT3 diffuse;
//		float pad2;
//		XMFLOAT3 specular;
//		float alpha;
//	};
//
//
//	Material material;
//
//	static Model* LoadFromOBJ(const string& modelname, bool smoothing = false);
//
//	static void SetDevice(ID3D12Device* device) { Model::device = device; }
//
//	void Draw(ID3D12GraphicsCommandList* commandList, UINT rootParamIndexMaterial);
//
//private:
//
//	static ID3D12Device* device;
//
//	void LoadFromOBJInternal(const string& modelname,bool smoothing);
//
//	void LoadMaterial(const string& directoryPath, const string& filename);
//
//	void LoadTexture(const string& directoryPath, const string& filename);
//
//	void CreateBuffers();
//
//	void AddSmoothData(unsigned short indexPosition, unsigned short indexVertex);
//
//	void CaliculateSmoothedVertexNormals();
//
//	vector<unsigned short>indices;
//	vector<VertexPos> vertices;
//
//	ComPtr<ID3D12Resource> vertBuff;
//
//	ComPtr<ID3D12Resource> indexBuff;
//
//	ComPtr<ID3D12Resource> texBuff;
//
//	D3D12_VERTEX_BUFFER_VIEW vbView;
//	D3D12_INDEX_BUFFER_VIEW ibView;
//	
//	ComPtr<ID3D12Resource> constBuffB1;
//
//	unordered_map<unsigned short, vector<unsigned short>>smoothData;
//
//	TextureData* tex;
//};
//
//*/
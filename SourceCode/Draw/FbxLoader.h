#pragma once

#pragma warning(push)
#pragma warning(disable:26495)
#pragma warning(disable:26812)
#pragma warning(disable:26451)
#include <fbxsdk.h>
#pragma warning(pop)


#include <d3d12.h>
#include <d3dx12.h>
#include<string>
#include"Model.h"
#include"TextureManager.h"

using namespace std;

class FbxLoader
{
private:

	using string = std::string;


public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();

	void Initialize(ID3D12Device* device);

	void Finalize();

	Model* LoadModelFlomFile(const string& modelname);

	void ParseNodeRecursive(Model* model, FbxNode* fbxNode, Node* parent = nullptr);
	/// <summary>
	/// メッシュ読み取り
	/// </summary>
	/// <param name="model">読み込み先モデルオブジェクト</param>
	/// <param name="fbxNode">解析対象のノード対象</param>
	void ParseMesh(Model* model, FbxNode* fbxNode);
	//頂点座標読み込み
	void ParseMeshVertices(Model* model, FbxMesh* fbxMesh);
	//面情報読み込み
	void ParseMeshFaces(Model* model, FbxMesh* fbxMesh);
	//マテリアル読み込み
	void ParseMaterial(Model* model, FbxNode* fbxNode);
	//スキニング情報の読み取り
	void ParseSkin(Model* model, FbxMesh* fbxMesh);
	//テクスチャ読み込み
	void LoadTexture(Model* model, const std::string& fullpath);

	static const string baseDirectory;
	//ディレクリを含んだファイルパスからファイル名を抽出する
	string ExtractFileName(const string& path);

	static void ConvertMatrixFromFbx(XMMATRIX* dst, const FbxAMatrix& src);

private:

	static const string defaultTextureFileName;

	ID3D12Device* device = nullptr;

	FbxManager* fbxManager = nullptr;

	FbxImporter* fbxImpoter = nullptr;

	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader& obj) = delete;
};
#include "LevelLoder.h"



#include <fstream>
#include <cassert>

LevelData* LevelLoder::LoadFile(const std::string& fileName)
{
	// 連結してフルパスを得る
	const std::string fullpath = kDefaultBaseDirectory + fileName + kExtension;

	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	std::string name = deserialized["name"].get<std::string>();
	// 正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	// レベルデータ格納用インスタンスを生成
	LevelData* levelData = new LevelData();

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"])
	{
		RecursiveAnalysis(levelData, object);
	}

	return levelData;
}

void LevelLoder::RecursiveAnalysis(LevelData* levelData, nlohmann::json objJson)
{
	assert(objJson.contains("type"));

	// 種別を取得
	std::string type = objJson["type"].get<std::string>();

	// MESH
	if (type.compare("MESH") == 0) {
		// 要素追加
		levelData->objects.emplace_back(LevelData::ObjectData{});
		// 今追加した要素の参照を得る
		LevelData::ObjectData& objectData = levelData->objects.back();

		if (objJson.contains("file_name")) {
			// ファイル名
			objectData.fileName = objJson["file_name"];
		}

		// トランスフォームのパラメータ読み込み
		nlohmann::json& transform = objJson["transform"];
		// 平行移動
		objectData.translation.m128_f32[0] = (float)transform["translation"][1];
		objectData.translation.m128_f32[1] = (float)transform["translation"][2];
		objectData.translation.m128_f32[2] = -(float)transform["translation"][0];
		objectData.translation.m128_f32[3] = 1.0f;
		// 回転角
		objectData.rotation.m128_f32[0] = -(float)transform["rotation"][1];
		objectData.rotation.m128_f32[1] = -(float)transform["rotation"][2];
		objectData.rotation.m128_f32[2] = (float)transform["rotation"][0];
		objectData.rotation.m128_f32[3] = 0.0f;
		// スケーリング
		objectData.scaling.m128_f32[0] = (float)transform["scaling"][1];
		objectData.scaling.m128_f32[1] = (float)transform["scaling"][2];
		objectData.scaling.m128_f32[2] = (float)transform["scaling"][0];
		objectData.scaling.m128_f32[3] = 0.0f;

		// TODO: コライダーのパラメータ読み込み
	}

	// TODO: オブジェクト走査を再帰関数にまとめ、再帰呼出で枝を走査する
	if (objJson.contains("children"))
	{
		for (nlohmann::json& child : objJson["children"]) RecursiveAnalysis(levelData, child);
	}
}

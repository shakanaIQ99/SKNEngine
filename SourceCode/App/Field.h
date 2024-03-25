#pragma once
#include"ObjModel.h"
#include"OBJ3D.h"
#include"Camera.h"

class Field 
{
public:

	void Init(Camera* _camera);

	void Update();

	void Draw();

	static float GetArea();

	static bool OutOfArea(const Vector2& pos,const float scale=0.0f);

	float GetUpArea();

	/// <summary>
	/// ImGui関係
	/// </summary>
	void ImGuiSet();

private:

	Camera* camera;

	vector<std::unique_ptr<OBJ3D>> ground;
	std::unique_ptr<ObjModel> ground_model;

	const float UpAreaLimit = 100.0f;

	static const float LimitLine;

	static const float AreaLimit;

	const size_t LaneTileNum = 10;

	const float TileSize = AreaLimit / static_cast<float>(LaneTileNum);

	const size_t TileNum = LaneTileNum * LaneTileNum;

};


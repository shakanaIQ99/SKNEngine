#pragma once
#include"ObjModel.h"
#include"OBJ3D.h"
#include"Camera.h"

class Field 
{
public:

	void Initialize(Camera* _camera);

	void Update();

	void Draw();

	static float GetArea();

	static bool OutOfArea(const Vector2& pos,const float scale=0.0f);

	float GetUpArea();

	/// <summary>
	/// ImGui関係
	/// </summary>
	void ImGuiMode();

private:

	Camera* camera;

	vector<std::unique_ptr<OBJ3D>> ground;
	std::unique_ptr<ObjModel> ground_model;

	const float upAreaLimit = 100.0f;

	static const float limitLine;

	static const float areaLimit;

	const size_t laneTileNum = 10;

	const float tileSize = areaLimit / static_cast<float>(laneTileNum);

	const size_t tileNum = laneTileNum * laneTileNum;

};


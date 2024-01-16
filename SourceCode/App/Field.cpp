#include "Field.h"

#include"ImGuiManager.h"
const float Field::AreaLimit = 250.0f;	//半径

void Field::Init(Camera* _camera)
{

	ground.clear();
	ground_model.reset(ObjModel::LoadFromOBJ("555"));

	for (size_t i = 0; i < TileNum; i++)
	{
		std::unique_ptr<OBJ3D > g = std::make_unique<OBJ3D>();

		ground.push_back(std::move(g));
	}

	for (auto& Ground : ground)
	{
		Ground.reset(OBJ3D::Create());
		Ground->SetModel(ground_model.get());
		Ground->Wt.scale_ = { TileSize / 2.0f,0.0f,TileSize / 2.0f };
	}

	for (size_t i = 0; i < ground.size(); i++)
	{
		ground[i]->Wt.translation_.x = -((AreaLimit / 2.0f) - (TileSize / 2.0f)) + ((TileSize) * static_cast<float>(i % 10));
		ground[i]->Wt.translation_.y = 0;
		ground[i]->Wt.translation_.z = ((AreaLimit / 2.0f) - (TileSize / 2.0f)) - ((TileSize) * static_cast<float>(i / 10));
	}

	camera = _camera;
}

void Field::Update()
{
	for (auto& Ground : ground)
	{
		Ground->Update(camera->getView());
	}
#ifdef _DEBUG
	ImGuiSet();
#endif


}

void Field::Draw()
{
	for (auto& Ground : ground)
	{
		Ground->Draw();
	}

}

float Field::GetArea()
{
	return AreaLimit;
}

float Field::GetUpArea()
{
	return UpAreaLimit;
}

void Field::ImGuiSet()
{
	//ImguI
	ImGui::SetNextWindowPos({ ImGui::GetMainViewport()->WorkPos.x, ImGui::GetMainViewport()->WorkPos.y + 10 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize({ 400, 500 });

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoResize;
	ImGui::Begin("Field", NULL, window_flags);

	ImGui::Text("TileNum%d", ground.size());

	ImGui::End();
}

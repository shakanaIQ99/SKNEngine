#include "TitleScene.h"
#include "SpriteCommon.h"
#include "Input.h"
#include "SceneControl.h"
#include "ImGuiManager.h"
#include "Easing.h"

template <typename T, std::size_t N>
struct Array
{
	T data[N];
	const size_t size() const { return N; }

	const T operator [] (size_t index) const { return data[index]; }
	operator T* () {
		T* p = new T[N];
		memcpy(p, data, sizeof(data));
		return p;
	}
};

template <typename T, typename ... U> Array(T, U...)->Array<T, 1 + sizeof...(U)>;


struct GraphEditorDelegate : public GraphEditor::Delegate
{
	bool AllowedLink([[maybe_unused]] GraphEditor::NodeIndex from, [[maybe_unused]] GraphEditor::NodeIndex to) override
	{
		return true;
	}

	void SelectNode(GraphEditor::NodeIndex nodeIndex, bool selected) override
	{
		mNodes[nodeIndex].mSelected = selected;
	}

	void MoveSelectedNodes(const ImVec2 delta) override
	{
		for (auto& node : mNodes)
		{
			if (!node.mSelected)
			{
				continue;
			}
			node.x += delta.x;
			node.y += delta.y;
		}
	}

	virtual void RightClick([[maybe_unused]] GraphEditor::NodeIndex nodeIndex, [[maybe_unused]] GraphEditor::SlotIndex slotIndexInput, [[maybe_unused]] GraphEditor::SlotIndex slotIndexOutput) override
	{
	}

	void AddLink(GraphEditor::NodeIndex inputNodeIndex, GraphEditor::SlotIndex inputSlotIndex, GraphEditor::NodeIndex outputNodeIndex, GraphEditor::SlotIndex outputSlotIndex) override
	{
		mLinks.push_back({ inputNodeIndex, inputSlotIndex, outputNodeIndex, outputSlotIndex });
	}

	void DelLink(GraphEditor::LinkIndex linkIndex) override
	{
		mLinks.erase(mLinks.begin() + linkIndex);
	}

	void CustomDraw(ImDrawList* drawList, ImRect rectangle, [[maybe_unused]] GraphEditor::NodeIndex nodeIndex) override
	{
		drawList->AddLine(rectangle.Min, rectangle.Max, IM_COL32(0, 0, 0, 255));
		drawList->AddText(rectangle.Min, IM_COL32(255, 128, 64, 255), "Draw");
	}

	const size_t GetTemplateCount() override
	{
		return sizeof(mTemplates) / sizeof(GraphEditor::Template);
	}

	const GraphEditor::Template GetTemplate(GraphEditor::TemplateIndex index) override
	{
		return mTemplates[index];
	}

	const size_t GetNodeCount() override
	{
		return mNodes.size();
	}

	const GraphEditor::Node GetNode(GraphEditor::NodeIndex index) override
	{
		const auto& myNode = mNodes[index];
		return GraphEditor::Node
		{
			myNode.name,
			myNode.templateIndex,
			ImRect(ImVec2(myNode.x, myNode.y), ImVec2(myNode.x + 200, myNode.y + 200)),
			myNode.mSelected
		};
	}

	const size_t GetLinkCount() override
	{
		return mLinks.size();
	}

	const GraphEditor::Link GetLink(GraphEditor::LinkIndex index) override
	{
		return mLinks[index];
	}

	// Graph datas
	static const inline GraphEditor::Template mTemplates[] = {
		{
			IM_COL32(160, 160, 180, 255),
			IM_COL32(100, 100, 140, 255),
			IM_COL32(110, 110, 150, 255),
			1,
			Array{"MyInput"},
			nullptr,
			6,
			Array{"MyOutput0", "MyOuput1"},
			nullptr
		},

		{
			IM_COL32(180, 160, 160, 255),
			IM_COL32(140, 100, 100, 255),
			IM_COL32(150, 110, 110, 255),
			3,
			nullptr,
			Array{ IM_COL32(200,100,100,255), IM_COL32(100,200,100,255), IM_COL32(100,100,200,255) },
			1,
			Array{"MyOutput0"},
			Array{ IM_COL32(200,200,200,255)}
		}
	};

	struct Node
	{
		const char* name;
		GraphEditor::TemplateIndex templateIndex;
		float x, y;
		bool mSelected;
	};

	std::vector<Node> mNodes = {
		{
			"My Node 0",
			0,
			0, 0,
			true
		},

		{
			"My Node 1",
			0,
			400, 0,
			false
		},

		{
			"My Node 2",
			1,
			400, 400,
			false
		}
	};

	std::vector<GraphEditor::Link> mLinks = { {0, 0, 1, 0} };
};


void TitleScene::Initialize()
{
	preTitle = std::make_unique<Sprite2D>();
	preTitle->Initialize("Title");
	preTitle->Wt.translation_ = { DxWindow::window_width / 2.0f,DxWindow::window_height / 4.5f ,0.0f };

	preTitle2 = std::make_unique<Sprite2D>();
	preTitle2->Initialize("Title2");
	preTitle2->Wt.translation_ = { DxWindow::window_width / 2.0f,(DxWindow::window_height / 2.0f) + 100.0f ,0.0f };

	SceneItem[0] = std::make_unique<Sprite2D>();
	SceneItem[0]->Initialize("Scene");
	SceneItem[1] = std::make_unique<Sprite2D>();
	SceneItem[1]->Initialize("Scene");

	light = LightGroup::Create();
	OBJ3D::SetLight(light);
	camera.Initialize();

	skydome_model.reset(ObjModel::LoadFromOBJ("skydome", true));
	skydome.reset(OBJ3D::Create());
	skydome->SetModel(skydome_model.get());

	ship_model.reset(ObjModel::LoadFromOBJ("Ship"));
	ship.reset(OBJ3D::Create());
	ship->SetModel(ship_model.get());

	titlePlayer_model.reset(ObjModel::LoadFromOBJ("Player"));
	titlePlayer.reset(OBJ3D::Create());
	titlePlayer->SetModel(titlePlayer_model.get());

	camera.Reset();

	camera.SetPos(Vector3(30.0f, 15.0f, 100.0f));
	camera.SetRotate(Vector3(0, 3.5f, 0));
	camera.SetTarget(Vector3(0, 0, 0));
	titlePlayer->Wt.scale_ = { 4.0f, 4.0f, 4.0f };
	titlePlayer->Wt.translation_.y = titlePlayer->Wt.scale_.y * 2.0f;

	sceneState = TitleState::TITLEBASE;
	sceneYpos = 0;


}

void TitleScene::Update()
{
	switch (sceneState)
	{
	case TitleState::TITLEBASE:
		if (tenmetu > 254.0f || tenmetu < 0.0f)
		{
			decri *= -1.0f;
		}
		tenmetu += decri;

		if (Input::GetPadButtonDown(XINPUT_GAMEPAD_A) || Input::GetPressKey(DIK_END))
		{
			tenmetu = 255.0f;
			sceneState = TitleState::TITLESTART;
			preTarget = camera.GetView()->Gettarget();
			camera.SetTargetFlag(true);
		}

		break;
	case TitleState::TITLESTART:


		sceneCameraTimer++;

		sceneYpos = EaseOutQuint(0, 100.0f, static_cast<float>(sceneCameraTimer) , static_cast<float>(sceneCameraTime));
		tenmetu = EaseOutQuint(255.0f, 0, static_cast<float>(sceneCameraTimer), static_cast<float>(sceneCameraTime));
		preTitle->Wt.color = { tenmetu / 255.0f ,tenmetu / 255.0f ,tenmetu / 255.0f ,tenmetu / 255.0f };


		if (sceneCameraTimer > sceneCameraTime)
		{
			sceneState = TitleState::TITLEMOVIE;
		}


		break;

	case TitleState::TITLEMOVIE:

		titlePlayer->Wt.translation_.z = EaseOutSine(0, 1000.0f, static_cast<float>(sceneMoveTimer), static_cast<float>(sceneMoveTime));
		sceneMoveTimer++;
		if (sceneMoveTimer > sceneMoveTime)
		{
			sceneState = TitleState::TITLEEND;
		}

		break;

	case TitleState::TITLEEND:

		SceneControl::GetInstance()->Change("GAME");

		break;
	}

#ifdef _DEBUG
	ImGuiMode();
#endif
	camera.SetTarget(Vector3(EaseOutQuint(preTarget, titlePlayer->Wt.translation_, static_cast<float>(sceneCameraTimer), static_cast<float>(sceneCameraTime))));
	preTitle2->Wt.color = { tenmetu / 255.0f ,tenmetu / 255.0f ,tenmetu / 255.0f ,tenmetu / 255.0f };
	camera.Update();
	skydome->Update(camera.GetView());
	ship->Update(camera.GetView());
	titlePlayer->Update(camera.GetView());
	preTitle->Update();
	preTitle2->Update();

	SceneItem[0]->Update();
	SceneItem[1]->Update();
}

void TitleScene::Draw()
{

	skydome->Draw();
	ship->Draw();
	titlePlayer->Draw();

	SpriteCommon::PreDraw();
	preTitle->Draw();
	preTitle2->Draw();
	SceneItem[0]->Draw(0, 0, 1280.0f, sceneYpos);
	SceneItem[1]->Draw(0,720.0f - sceneYpos, 1280.0f, 720.0f);


}

void TitleScene::Finalize()
{
	delete light;
}

void TitleScene::ImGuiMode()
{
	//ImguI
	ImGui::SetNextWindowPos({ ImGui::GetMainViewport()->WorkPos.x + 800, ImGui::GetMainViewport()->WorkPos.y + 10 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize({ 400, 500 });

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoResize;
	ImGui::Begin("Player", NULL, window_flags);

	ImGui::Text("Position");
	ImGui::DragFloat("Y", &titlePlayer->Wt.translation_.y, 0.5f);

	// Graph Editor
	static GraphEditor::Options options;
	static GraphEditorDelegate delegate;
	static GraphEditor::ViewState viewState;
	static GraphEditor::FitOnScreen fit = GraphEditor::Fit_None;
	static bool showGraphEditor = true;

	if (ImGui::CollapsingHeader("Graph Editor"))
	{
		ImGui::Checkbox("Show GraphEditor", &showGraphEditor);
		GraphEditor::EditOptions(options);
	}
	
	ImGui::End();

	if (showGraphEditor)
	{
		ImGui::Begin("Graph Editor", NULL, 0);
		if (ImGui::Button("Fit all nodes"))
		{
			fit = GraphEditor::Fit_AllNodes;
		}
		ImGui::SameLine();
		if (ImGui::Button("Fit selected nodes"))
		{
			fit = GraphEditor::Fit_SelectedNodes;
		}
		GraphEditor::Show(delegate, options, viewState, true, &fit);

		ImGui::End();
	}
}



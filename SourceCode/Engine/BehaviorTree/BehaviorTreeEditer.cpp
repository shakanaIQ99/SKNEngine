#include "BehaviorTreeEditer.h"
#pragma warning (push)
#pragma warning (disable:4245)
#pragma warning (disable:4189)
#include "GraphEditor.h"
#pragma warning (pop)

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

template <typename T, typename ... U> Array(T, U...) -> Array<T, 1 + sizeof...(U)>;


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
		//Root
		{
			IM_COL32(160, 160, 180, 255),
			IM_COL32(100, 100, 140, 255),
			IM_COL32(110, 110, 150, 255),
			0,
			Array{"MyInput"},
			nullptr,
			1,
			nullptr,
			nullptr
		},
		//Sequencer
		{
			IM_COL32(255, 255, 255, 255),
			IM_COL32(0, 255, 0, 255),
			IM_COL32(0, 0, 0, 255),
			1,
			nullptr,
			Array{ IM_COL32(200,100,100,255), IM_COL32(100,200,100,255), IM_COL32(100,100,200,255) },
			3,
			nullptr,
			Array{ IM_COL32(200,200,200,255)}
		},
		//Action
		{
			IM_COL32(255, 255, 255, 255),
			IM_COL32(255, 0, 0, 255),
			IM_COL32(255, 255, 0, 255),
			1,
			nullptr,
			nullptr,
			0,
			nullptr,
			nullptr
		},
		//Selector
		{
			IM_COL32(180, 160, 160, 255),
			IM_COL32(0, 0, 255, 255),
			IM_COL32(0, 0, 0, 255),
			1,
			nullptr,
			nullptr,
			2,
			nullptr,
			nullptr
		},

		//Condition
		{
			IM_COL32(180, 160, 160, 255),
			IM_COL32(0, 255, 255, 255),
			IM_COL32(0, 0, 0, 255),
			1,
			nullptr,
			nullptr,
			1,
			nullptr,
			nullptr
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
			"Root",
			0,
			0, 0,
			true
		},//0

		{
			"Selector",
			3,
			400, 0,
			false
		},//1

		{
			"Leave",
			2,
			800, 400,
			false
		},//2

		{
			"Sequencer",
			1,
			800, 0,
			false
		},//3

		{
			"CrossLange",
			4,
			1200, 400,
			false
		},//4

		{
			"Closs",
			2,
			1600, 400,
			false
		},//5

		{
			"FanShape",
			2,
			1200, 0,
			false
		},//6

		{
			"Step",
			2,
			1200, -400,
			false
		},//7

		{
			"FarLange",
			4,
			1200, 400,
			false
		},//4

	};

	std::vector<GraphEditor::Link> mLinks = { {0, 0, 1, 0},{1, 1, 2, 0},{1, 0, 3, 0},{3, 2, 4, 0},{4, 0, 5, 0},{3, 1, 6, 0},{3, 0, 7, 0} };
};

void BehaviorTreeEditer::Draw()
{
	//ImguI
	ImGui::SetNextWindowPos({ ImGui::GetMainViewport()->WorkPos.x + 400, ImGui::GetMainViewport()->WorkPos.y + 10 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize({ 400, 500 });

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoResize;
	ImGui::Begin("BTEditer", NULL, window_flags);
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
		if (ImGui::Button("Save"))
		{

		}
		GraphEditor::Show(delegate, options, viewState, true, &fit);

		ImGui::End();
	}
}

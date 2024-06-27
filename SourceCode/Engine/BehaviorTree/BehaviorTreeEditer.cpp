#include "BehaviorTreeEditer.h"
#pragma warning (push)
#pragma warning (disable:4245)
#pragma warning (disable:4189)
#include "GraphEditor.h"
#pragma warning (pop)
#include <json.hpp>
#include <fstream>
#include <string>
using namespace nlohmann;
using namespace std;


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
		//drawList->AddLine(rectangle.Min, rectangle.Max, IM_COL32(0, 0, 0, 255));
		drawList->AddText(rectangle.Min, IM_COL32(255, 128, 64, 255), mNodes[nodeIndex].nodeTypeName);
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

	const EGNode GetEGNode(GraphEditor::NodeIndex index)
	{
		return mNodes[index];
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

	//struct Node
	//{
	//	const char* name;
	//	GraphEditor::TemplateIndex templateIndex;
	//	float x, y;
	//	bool mSelected;
	//};

	std::vector<EGNode> mNodes = { {"Root",0,0,0,false,"root"} };

	std::vector<GraphEditor::Link> mLinks/* = { {0, 0, 1, 0},{1, 1, 2, 0},{1, 0, 3, 0},{3, 2, 4, 0},{4, 0, 5, 0},{3, 1, 6, 0},{3, 0, 7, 0} }*/;
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
		ImGui::Begin("Graph Editor", NULL, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save"))
				{
					GetInstance()->Save(delegate);
				}
				if (ImGui::MenuItem("Load")) {
					LoadFile(GetInstance()->filePath,delegate);
				}

				ImGui::EndMenu();
			}
		}
		ImGui::EndMenuBar();
		if (ImGui::Button("Fit all nodes"))
		{
			fit = GraphEditor::Fit_AllNodes;
		}
		ImGui::SameLine();
		if (ImGui::Button("Fit selected nodes"))
		{
			fit = GraphEditor::Fit_SelectedNodes;
		}
	
		static int templateNum = 0;
		const char* templateName[] = { "Root","Sequencer","Action","Selector","Condition","Loop" };
		ImGui::Combo("##MovemodeNumCombo", &templateNum, templateName, IM_ARRAYSIZE(templateName));
		static int NodeNum = 0;
		const char* NodeName[] = { "aaaa","bbbb","cccc","rrrr","dddd","ssss" };
		ImGui::Combo("##MovemodeNumCombo", &NodeNum, NodeName, IM_ARRAYSIZE(NodeName));
		if (ImGui::Button("CreateNode"))
		{
			delegate.mNodes.push_back(EGNode(templateName[templateNum], templateNum,0, 0,false,NodeName[NodeNum]));
			
		}
		if (ImGui::Button("DeleteNode"))
		{
			for (size_t i = 0; i < delegate.mNodes.size(); i++)
			{
				std::erase_if(delegate.mNodes, [](EGNode node) { return node.mSelected == true; });
			}
		}
	/*	if (ImGui::Button("Save"))
		{

		}*/
		GraphEditor::Show(delegate, options, viewState, true, &fit);

		ImGui::End();
	}
}

void BehaviorTreeEditer::LoadFile(std::string filePath, GraphEditorDelegate Delegate)
{
	
	std::ifstream file;

	file.open(filePath);

	if (file.fail())
	{
		assert(0);
	}

	json deserialized;
	file >> deserialized;

	assert(deserialized.is_object());
	assert(deserialized.contains("NodeCount"));
	assert(deserialized["NodeCount"].is_string());

	


	//ツリーの構築
	//読み込みの再起関数
	function<void(const json& object)> processNode = [&](const json& object)
		{

			Delegate.mNodes.push_back(EGNode(object["NodeType"].get<string>().c_str(), object["NodeParam"].get<size_t>(), std::stof(object["NodePosX"].get<string>()), std::stof(object["NodePosY"].get<string>()), false, "aaa"));
			
		};


	file.close();
}

void BehaviorTreeEditer::Save(GraphEditor::Delegate& delegate)
{
	//json形式のstringを作成

//作成イメージ
// {
// "Node":{
//	"NodeType":"NODETYPE",
//	"NodeParam":"NODEPARAM",
//  "Children":[
//			{"Node": {
//				}
//			},
//			{"Node": {
//				}
//			},
//		]
//	}
// }

	stringstream jsonStr;

	jsonStr << "\"NodeCount\":\""<< delegate.GetNodeCount() <<"\",\n";
	jsonStr << "\"LinkCount\":\"" << delegate.GetLinkCount() << "\",\n";

	jsonStr << "{" << endl;
	

	for (size_t i = 0; i < delegate.GetNodeCount(); i++)
	{
		jsonStr << "{" << endl;
		jsonStr << "\"NodeType\":\"" << delegate.GetNode(i).mName << "\",\n";
		jsonStr <<  "\"NodeParam\":\"" << delegate.GetNode(i).mTemplateIndex << "\",\n";

		
		
		jsonStr <<  "\"NodePosX\":\"" << delegate.GetNode(i).mRect.Min.x << "\",\n";
		jsonStr <<  "\"NodePosY\":\"" << delegate.GetNode(i).mRect.Min.y << "\",\n";
		
		jsonStr << "}\n";

	};


	for (size_t i = 0; i < delegate.GetLinkCount(); i++)
	{
		
		jsonStr << "\"NodeLink"<< "\":{";
		jsonStr <<  delegate.GetLink(i).mInputNodeIndex << ",";
		jsonStr <<  delegate.GetLink(i).mInputSlotIndex << ",";
		jsonStr <<  delegate.GetLink(i).mOutputNodeIndex << ",";
		jsonStr <<  delegate.GetLink(i).mOutputSlotIndex << ",";
		jsonStr << "}\n";

	};
	
	//最後のカンマを削除
	string tempStr = jsonStr.str();

	tempStr.pop_back();
	tempStr.pop_back();

	tempStr += string("\n");

	jsonStr.str("");
	jsonStr.clear(stringstream::goodbit);

	jsonStr << tempStr;
	jsonStr << "\n";

	jsonStr << "}" << endl;


	//ファイル書き込み
	std::ofstream file;

	file.open("Resources/Boss.json");

	if (file.fail())
	{
		assert(0);
	}

	file << jsonStr.str() << endl;

	file.close();
}



BehaviorTreeEditer* BehaviorTreeEditer::GetInstance()
{
	static BehaviorTreeEditer instance;
	return &instance;
}

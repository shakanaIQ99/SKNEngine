#include "BehaviorTree.h"
#include "RootNode.h"
#include <json.hpp>
#include <functional>
#include <fstream>
#include <string>
#include "ActionNode.h"
#include "LoopNode.h"
#include "SequencerNode.h"
#include "SelectorNode.h"
#include "ConditionNode.h"
#include "EditerNode.h"

using namespace nlohmann;
using namespace std;

SKN::BehaviorTree::BehaviorTree()
{
	root = std::make_unique<RootNode>();
	dynamic_cast<RootNode*>(root.get())->SetRootBT(this);
}

void SKN::BehaviorTree::SetFactory(BehaviorTreeFactory& factory)
{
	funcFactory.reset(&factory);
}  



void SKN::BehaviorTree::Tick()
{
	root->Update();
}

void SKN::BehaviorTree::LoadJson(std::string path)
{
	ifstream file;

	file.open(path);

	if (file.fail())
	{
		assert(0);
	}

	json deserialized;
	file >> deserialized;

	root = std::make_unique<RootNode>();
	dynamic_cast<RootNode*>(root.get())->SetRootBT(this);

	assert(deserialized.is_object());
	assert(deserialized.contains("Node0"));
	assert(deserialized["Node0"]["NodeType"] == "Root");

	//ツリーの構築
	//読み込みの再起関数
	function<void(const json& object, NodeBase* parent)> processNode = [&](const json& object, NodeBase* parent)
		{
			string nodeType = object["NodeType"].get<string>();
			//NodeTypeによって分岐
			if (nodeType == "Action")
			{
				parent->AddNode<ActionNode>(object["NodeParam"].get<string>());
			}
			else if (nodeType == "Loop")
			{
				parent->AddNode<LoopNode>(object["NodeParam"].get<string>());
			}
			else if (nodeType == "Selector")
			{
				parent->AddNode<SelectorNode>(object["NodeParam"].get<string>());
			}
			else if (nodeType == "Sequencer")
			{
				parent->AddNode<SequencerNode>(object["NodeParam"].get<string>());
			}
			else if (nodeType == "Condition")
			{
				parent->AddNode<ConditionNode>(object["NodeParam"].get<string>());
			}
			else
			{
				parent->AddNode<SequencerNode>("");
			}

			//子ノードの読み込み
			if (object.contains("Children"))
			{

				for (auto& node : object["Children"])
				{
					for (auto& singleNode : node)
					{
						processNode(singleNode, parent->Last());
					}
				}
			}
		};

	//読み込み再起関数呼び出し
	if (deserialized["Node0"].contains("Children"))
	{
		for (auto& node : deserialized["Node0"]["Children"])
		{
			processNode(node["Node0"], root.get());
		}
	}

	file.close();
}

void SKN::BehaviorTree::SaveJson(std::string path)
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

	jsonStr << "{" << endl;

	//ノードを処理する再起関数
	function<void(NodeBase* currentNode, std::string indent, std::string nodeNum)> processNode = [&](NodeBase* current, std::string indent, std::string nodeNum = "") {
		jsonStr << indent << "\"Node" << nodeNum << "\":{\n";
		string firstIndent = indent;
		indent += "  ";
		jsonStr << indent << "\"NodeType\":\"" << current->GetNodeType() << "\",\n";
		jsonStr << indent << "\"NodeParam\":\"" << current->GetParam() << "\",\n";

		if (current->editorNodePtr)
		{
			jsonStr << indent << "\"NodePosX\":\"" << current->editorNodePtr->GetPos().x << "\",\n";
			jsonStr << indent << "\"NodePosY\":\"" << current->editorNodePtr->GetPos().y << "\",\n";
		}

		int32_t nodeNumInt = 0;
		if (current->GetChildren().size())
		{
			jsonStr << indent << "\"Children\":[\n" << indent << "{\n";

			for (auto& c : current->GetChildren())
			{
				processNode(c.get(), indent + "  ", to_string(nodeNumInt));
				nodeNumInt++;
			}
			//最後のカンマを削除
			string tempStr = jsonStr.str();

			tempStr.pop_back();
			tempStr.pop_back();

			tempStr += string("\n");

			jsonStr.str("");
			jsonStr.clear(stringstream::goodbit);

			jsonStr << tempStr;

			jsonStr << indent << "}],\n";
		}

		jsonStr << indent << "\"NumChildren\":\"" << nodeNumInt << "\"\n";

		jsonStr << firstIndent << "},\n";
		};

	//ルートノードから呼び出し
	processNode(root.get(), "", "0");

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

	file.open(path);

	if (file.fail())
	{
		assert(0);
	}

	file << jsonStr.str() << endl;

	file.close();
}

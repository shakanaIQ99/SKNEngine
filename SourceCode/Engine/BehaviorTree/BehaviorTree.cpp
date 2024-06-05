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
}

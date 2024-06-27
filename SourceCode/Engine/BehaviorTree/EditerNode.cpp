#include "EditerNode.h"
#include"BehaviorTree.h"
#include"GraphEditor.h"

static std::vector<std::string> itemList
{
	"Action",
	"Sequencer",
	"Selector",
	"Loop",
	"Condition",
	"Root"
};

EditerNode::EditerNode(std::unique_ptr<SKN::NodeBase>* Node, std::string UniqueName, BehaviorTreeEditer* Master, GraphEditor::TemplateIndex TemplateIndex)
{
	node = Node;
	eNode.name = UniqueName.c_str();
	eNode.x = firstPos.x;
	eNode.y = firstPos.y;
	eNode.templateIndex = TemplateIndex;
	(*node)->editorNodePtr = this;

	uniqueName = UniqueName;

	master = Master;
}

void EditerNode::ChangeNodeType(std::string type)
{
	std::unique_ptr<SKN::NodeBase>* lastNode = std::move(node);

	SKN::NodeBase* last = lastNode->get();

	std::string param = last->GetParam();
	SKN::NodeBase* parent = last->parent;
	auto children = std::move(last->children);
	auto parentBT = last->parentBT;

	//NodeTypeによって分岐
	if (type == "Action")
	{
		*node = std::make_unique<SKN::ActionNode>();
	}
	else if (type == "Loop")
	{
		*node = std::make_unique<SKN::LoopNode>();
	}
	else if (type == "Selector")
	{
		*node = std::make_unique<SKN::SelectorNode>();
	}
	else if (type == "Sequencer")
	{
		*node = std::make_unique<SKN::SequencerNode>();
	}
	else if (type == "Condition")
	{
		*node = std::make_unique<SKN::ConditionNode>();
	}
	else
	{
		*node = std::make_unique<SKN::RootNode>();
	}

	(*node)->SetParam(param);
	(*node)->editorNodePtr = this;
	(*node)->parentBT = parentBT;
	(*node)->children = std::move(children);
	(*node)->parent = parent;

	for (auto& c : (*node)->children)
	{
		c->parent = node->get();
	}
}


void EditerNode::Draw()
{
}

const Vector2& EditerNode::GetPos()
{
	pos.x = eNode.x;
	pos.y = eNode.y;
	return pos;
}

void EditerNode::SetComboBoxItem(std::string nodeType)
{
	const char* cur = nullptr;
	for (auto& i : itemList)
	{
		if (i == nodeType)
		{
			cur = i.c_str();
		}
	}

	currentItem = cur;
}

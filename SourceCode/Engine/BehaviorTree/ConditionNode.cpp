#include "ConditionNode.h"
#include "BehaviorTree.h"
void SKN::ConditionNode::OnStart()
{
}

SKN::NodeStatus SKN::ConditionNode::Update()
{
	bool doesConditionMatch = parentBT->GetFactory()->GetConditionFunc(param)();
	if (selectedIndex < 0)
	{
		if (doesConditionMatch)
		{
			selectedIndex = 0;
		}
		else if (children.size() >= 2)
		{
			selectedIndex = 1;
		}
	}

	NodeStatus childStatus = NodeStatus::Inactive;
	int32_t ind = 0;
	for (auto itr = children.begin(); itr != children.end(); itr++)
	{
		if (ind == selectedIndex)
		{
			childStatus = (*itr)->Update();
		}
		ind++;
	}

	if (childStatus == NodeStatus::Completed || childStatus == NodeStatus::Success)
	{
		InitNode();
		return NodeStatus::Success;
	}

	if (childStatus == NodeStatus::Failure)
	{
		InitNode();
		return NodeStatus::Failure;
	}

	return NodeStatus::Running;
}

void SKN::ConditionNode::OnEnd()
{
}

void SKN::ConditionNode::OnAbort()
{
}

void SKN::ConditionNode::SetParam(std::string Param)
{
	NodeBase::SetParam(param);
}

void SKN::ConditionNode::InitNode()
{
	selectedIndex = -1;
}

std::string SKN::ConditionNode::GetNodeType()
{
	return "Condition";
}

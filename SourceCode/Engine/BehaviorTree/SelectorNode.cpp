#include "SelectorNode.h"
#include <random>
void SelectorNode::OnStart()
{
}

NodeStatus SelectorNode::Update()
{
	if (children.size() < 0)
	{
		InitNode();
		return NodeStatus::Success;
	}

	if (selectedIndex < 0)
	{
		std::random_device rd;
		std::default_random_engine eng(rd());
		std::uniform_real_distribution<int> distr(0, static_cast<int>(children.size())-1);

		selectedIndex = distr(eng);
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

void SelectorNode::OnEnd()
{
}

void SelectorNode::OnAbort()
{
}

void SelectorNode::SetParam(std::string Param)
{
	NodeBase::SetParam(param);
}

void SelectorNode::InitNode()
{
	selectedIndex = -1;
}

std::string SelectorNode::GetNodeType()
{
	return "Selector";
}

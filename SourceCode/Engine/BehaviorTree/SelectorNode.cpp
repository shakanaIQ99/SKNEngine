#include "SelectorNode.h"

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
		//selectedIndex = Util::RNG(0, (int32_t)children.size() - 1, true);
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
}

void SelectorNode::InitNode()
{
}

std::string SelectorNode::GetNodeType()
{
	return std::string();
}

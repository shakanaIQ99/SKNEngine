#include "SelectorNode.h"
#include <random>
#include <iostream>
void SKN::SelectorNode::OnStart()
{
}

SKN::NodeStatus SKN::SelectorNode::Update()
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
		std::uniform_int_distribution<int> distr(0, static_cast<int>(children.size())-1);

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

void SKN::SelectorNode::OnEnd()
{
}

void SKN::SelectorNode::OnAbort()
{
}

void SKN::SelectorNode::SetParam(std::string Param)
{
	NodeBase::SetParam(param);
}

void SKN::SelectorNode::InitNode()
{
	selectedIndex = -1;
}

std::string SKN::SelectorNode::GetNodeType()
{
	return "Selector";
}

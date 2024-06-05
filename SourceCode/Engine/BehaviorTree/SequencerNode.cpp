#include "SequencerNode.h"

void SKN::SequencerNode::OnStart()
{
}

SKN::NodeStatus SKN::SequencerNode::Update()
{
	if (children.size() <= activeIndex)
	{
		InitNode();
		return NodeStatus::Success;
	}

	NodeStatus childStatus = NodeStatus::Inactive;;
	uint32_t ind = 0;
	for (auto itr = children.begin(); itr != children.end(); itr++)
	{
		if (ind == activeIndex)
		{
			childStatus = (*itr)->Update();
		}
		ind++;
	}

	if (childStatus == NodeStatus::Completed || childStatus == NodeStatus::Success)
	{
		activeIndex++;
		if (activeIndex >= children.size())
		{
			InitNode();
			return NodeStatus::Success;
		}

		return NodeStatus::Running;
	}

	if (childStatus == NodeStatus::Failure)
	{
		InitNode();
		return NodeStatus::Failure;
	}

	return NodeStatus::Running;
}

void SKN::SequencerNode::OnEnd()
{
}

void SKN::SequencerNode::OnAbort()
{
}

void SKN::SequencerNode::SetParam(std::string Param)
{
	NodeBase::SetParam(param);
}

void SKN::SequencerNode::InitNode()
{
	activeIndex = 0;
}

std::string SKN::SequencerNode::GetNodeType()
{
	return "Sequencer";
}

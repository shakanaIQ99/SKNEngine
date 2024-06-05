#include "RootNode.h"

void SKN::RootNode::OnStart()
{
}

SKN::NodeStatus SKN::RootNode::Update()
{
	if (children.size())
	{
		return children.front()->Update();
	}

	return NodeStatus::Success;
}

void SKN::RootNode::OnEnd()
{
}

void SKN::RootNode::OnAbort()
{
	NodeBase::OnAbort();
}

void SKN::RootNode::SetRootBT(BehaviorTree* bt)
{
	parentBT = bt;
}

void SKN::RootNode::SetParam(std::string Param)
{
	NodeBase::SetParam(Param);
}

std::string SKN::RootNode::GetNodeType()
{
	return "Root";
}

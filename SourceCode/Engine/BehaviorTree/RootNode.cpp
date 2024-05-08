#include "RootNode.h"

void RootNode::OnStart()
{
}

NodeStatus RootNode::Update()
{
	if (children.size())
	{
		return children.front()->Update();
	}

	return NodeStatus::Success;
}

void RootNode::OnEnd()
{
}

void RootNode::OnAbort()
{
	NodeBase::OnAbort();
}

void RootNode::SetRootBT(BehaviorTree* bt)
{
	parentBT = bt;
}

void RootNode::SetParam(std::string Param)
{
	NodeBase::SetParam(Param);
}

std::string RootNode::GetNodeType()
{
	return "Root";
}

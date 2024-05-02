#include "RootNode.h"

void SKNBT::RootNode::OnStart()
{
}

SKNBT::NodeStatus SKNBT::RootNode::Update()
{
	if (children.size())
	{
		return children.front()->Update();
	}

	return NodeStatus::Success;
}

void SKNBT::RootNode::OnEnd()
{
}

void SKNBT::RootNode::OnAbort()
{
	NodeBase::OnAbort();
}

void SKNBT::RootNode::SetRootBT(BehaviorTree* bt)
{
	parentBT = bt;
}

void SKNBT::RootNode::SetParam(std::string Param)
{
	NodeBase::SetParam(Param);
}

std::string SKNBT::RootNode::GetNodeType()
{
	return "Root";
}

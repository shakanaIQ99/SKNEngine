#include "ActionNode.h"
#include "BehaviorTree.h"
void SKN::ActionNode::OnStart()
{
}

SKN::NodeStatus SKN::ActionNode::Update()
{
	return parentBT->GetFactory()->GetActionFunc(funcKey)();
}

void SKN::ActionNode::OnEnd()
{
}

void SKN::ActionNode::OnAbort()
{
}

void SKN::ActionNode::SetParam(std::string Param)
{
	NodeBase::SetParam(param);
	funcKey = param;
}

void SKN::ActionNode::InitNode()
{
}

std::string SKN::ActionNode::GetNodeType()
{
	return "Action";
}

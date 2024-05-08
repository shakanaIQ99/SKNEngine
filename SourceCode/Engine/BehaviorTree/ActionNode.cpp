#include "ActionNode.h"

void ActionNode::OnStart()
{
}

NodeStatus ActionNode::Update()
{
	return parentBT->GetFactory()->GetActionFunc(funcKey)();
}

void ActionNode::OnEnd()
{
}

void ActionNode::OnAbort()
{
}

void ActionNode::SetParam(std::string Param)
{
}

void ActionNode::InitNode()
{
}

std::string ActionNode::GetNodeType()
{
	return std::string();
}

#include "NodeBase.h"

void NodeBase::OnAbort()
{
	OnEnd();
}

void NodeBase::SetParam(std::string Param)
{
	param = Param;
}

std::string NodeBase::GetParam()
{
	return param;
}

NodeBase* NodeBase::GetParent()
{
	return parent;
}

const std::list<std::unique_ptr<NodeBase>>& NodeBase::GetChildren()
{
	return children;
}

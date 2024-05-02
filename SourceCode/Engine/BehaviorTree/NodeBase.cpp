#include "NodeBase.h"

void SKNBT::NodeBase::OnAbort()
{
	OnEnd();
}

void SKNBT::NodeBase::SetParam(std::string Param)
{
	param = Param;
}

std::string SKNBT::NodeBase::GetParam()
{
	return param;
}

SKNBT::NodeBase* SKNBT::NodeBase::GetParent()
{
	return parent;
}

const std::list<std::unique_ptr<SKNBT::NodeBase>>& SKNBT::NodeBase::GetChildren()
{
	return children;
}

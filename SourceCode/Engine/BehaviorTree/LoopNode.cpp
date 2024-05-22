#include "LoopNode.h"

void LoopNode::OnStart()
{
}

NodeStatus LoopNode::Update()
{
    if (loopCount < param || param == 0)
    {
       NodeStatus status = children.front()->Update();

        if (status == NodeStatus::Completed || status == NodeStatus::Success || status == NodeStatus::Failure)
        {
            loopCount++;
            if (loopCount >= param && param != 0)
            {
                InitNode();
                return NodeStatus::Completed;
            }
        }
    }

    return NodeStatus::Running;
}

void LoopNode::OnEnd()
{
}

void LoopNode::OnAbort()
{
}

void LoopNode::SetParam(std::string Param)
{
    NodeBase::SetParam(Param);
}

void LoopNode::InitNode()
{
    loopCount = 0;
}

std::string LoopNode::GetNodeType()
{
    return "Loop";
}

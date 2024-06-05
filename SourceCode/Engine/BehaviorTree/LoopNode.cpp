#include "LoopNode.h"

void SKN::LoopNode::OnStart()
{
}

SKN::NodeStatus SKN::LoopNode::Update()
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

void SKN::LoopNode::OnEnd()
{
}

void SKN::LoopNode::OnAbort()
{
}

void SKN::LoopNode::SetParam(std::string Param)
{
    NodeBase::SetParam(Param);
}

void SKN::LoopNode::InitNode()
{
    loopCount = 0;
}

std::string SKN::LoopNode::GetNodeType()
{
    return "Loop";
}

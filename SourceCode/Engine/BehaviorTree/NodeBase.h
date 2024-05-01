#pragma once
#include <list>
#include <memory>
#include <string>
#include "BehaviorTree.h"
enum class NodeStatus 
{
    Inactive,
    Success,
    Failure,
    Running,
    Completed
};

class NodeBase
{
protected:
    
    std::list<std::unique_ptr<NodeBase>> children;

    uint32_t activeIndex = 0;

    BehaviorTree* parentBT = nullptr;
    NodeBase* parent = nullptr;
    std::string param;
public:
    virtual void OnStart() = 0;
    virtual NodeStatus Update() = 0;
    virtual void OnEnd() = 0;
    virtual void OnAbort();

    virtual void SetParam(std::string Param);

    std::string GetParam();
    NodeBase* GetParent();
    const std::list<std::unique_ptr<NodeBase>>& GetChildren();

    virtual std::string GetNodeType() = 0;
};


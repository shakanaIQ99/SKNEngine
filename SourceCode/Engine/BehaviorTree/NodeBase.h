#pragma once
#include <list>
#include <memory>
#include <string>
#include "BehaviorTree.h"

class NodeStatus
{

public:
    enum STATUS
    {
        Inactive,
        Success,
        Failure,
        Running,
        Completed
    };

    NodeStatus() = default;
    constexpr NodeStatus(STATUS Status) : status(Status) { }


    constexpr bool operator==(NodeStatus a) const { return status == a.status; }
    constexpr bool operator!=(NodeStatus a) const { return status != a.status; }

private:
    STATUS status;
    
};

class BehaviorTree;
class EditerNode;

class NodeBase
{
public:

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

    
    EditerNode* editorNodePtr = nullptr;

    template <class NodeType>
    void AddNode(const std::string& param)
    {
        children.emplace_back(std::make_unique<NodeType>());
        Last()->SetParam(param);
        Last()->parentBT = parentBT;
        Last()->parent= this;
    };

    NodeBase* Last();
    std::unique_ptr<NodeBase>* LastPtr();

    //新しいNodeのuptrへのポインタを返す
    std::unique_ptr<NodeBase>* ChangeParent(NodeBase* newParent);

    NodeBase() {};
    virtual ~NodeBase() {};
};




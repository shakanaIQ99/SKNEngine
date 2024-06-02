#pragma once
#include "BehaviorTreeFactory.h"
#include "NodeBase.h"
#include <memory>
#include <list>
#include "ActionNode.h"
#include "LoopNode.h"
#include "SequencerNode.h"
#include "SelectorNode.h"
#include "ConditionNode.h"
#include "RootNode.h"

class BehaviorTreeFactory;
class NodeBase;

class BehaviorTree
{
public:
	BehaviorTree();

	void SetFactory(BehaviorTreeFactory& factory);
	BehaviorTreeFactory* GetFactory() const { return funcFactory.get(); }
	void Tick();

	std::unique_ptr<NodeBase> root;
	std::list<std::unique_ptr<NodeBase>> floatingNodes;

	void LoadJson(std::string path);
	void SaveJson(std::string path);


private:
	std::unique_ptr<BehaviorTreeFactory> funcFactory;
};


#pragma once
#include "BehaviorTreeFactory.h"
#include "NodeBase.h"
#include <memory>
#include <list>

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


private:
	std::unique_ptr<BehaviorTreeFactory> funcFactory;
};


#pragma once
#include "BehaviorTreeFactory.h"
#include "NodeBase.h"
#include <memory>


class BehaviorTree
{
public:

	BehaviorTree();

	void SetFactory(const BehaviorTreeFactory& factory);
	BehaviorTreeFactory* GetFactory();

	void Tick();

	std::unique_ptr<NodeBase> root;
	std::list<std::unique_ptr<NodeBase>> floatingNodes;


private:
	BehaviorTreeFactory funcFactory;
};


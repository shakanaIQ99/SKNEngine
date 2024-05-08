#include "BehaviorTree.h"
#include "RootNode.h"

BehaviorTree::BehaviorTree()
{
	root = std::make_unique<RootNode>();
	dynamic_cast<RootNode*>(root.get())->SetRootBT(this);
}

void BehaviorTree::SetFactory(BehaviorTreeFactory& factory)
{
	funcFactory.reset(&factory);
}



void BehaviorTree::Tick()
{
	root->Update();
}

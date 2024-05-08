#include "BehaviorTree.h"

BehaviorTree::BehaviorTree()
{
}

void BehaviorTree::SetFactory(const BehaviorTreeFactory& factory)
{
}

std::unique_ptr<BehaviorTreeFactory> BehaviorTree::GetFactory()
{
	return &funcFactory;
}



void BehaviorTree::Tick()
{
}

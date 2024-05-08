#include "BehaviorTreeFactory.h"

void BehaviorTreeFactory::RegisterCondition(std::string key, std::function<bool(void)> func)
{
	conditionTable.emplace(key, func);
}

void BehaviorTreeFactory::RegisterAction(std::string key, std::function<NodeStatus(void)>func)
{
	actionTable.emplace(key, func);
}

const std::function<NodeStatus(void)> BehaviorTreeFactory::GetActionFunc(const std::string& key)
{
	return actionTable.at(key);
}

const std::function<bool(void)> BehaviorTreeFactory::GetConditionFunc(const std::string& key)
{
	return conditionTable.at(key);
}

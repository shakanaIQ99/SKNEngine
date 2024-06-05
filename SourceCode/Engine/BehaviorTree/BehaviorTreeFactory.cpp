#include "BehaviorTreeFactory.h"

void SKN::BehaviorTreeFactory::RegisterCondition(std::string key, std::function<bool(void)> func)
{
	conditionTable.emplace(key, func);
}

void SKN::BehaviorTreeFactory::RegisterAction(std::string key, std::function<SKN::NodeStatus(void)>func)
{
	actionTable.emplace(key, func);
}


const std::function<SKN::NodeStatus(void)> SKN::BehaviorTreeFactory::GetActionFunc(const std::string& key)
{
	return actionTable.at(key);
}

const std::function<bool(void)> SKN::BehaviorTreeFactory::GetConditionFunc(const std::string& key)
{
	return conditionTable.at(key);
}

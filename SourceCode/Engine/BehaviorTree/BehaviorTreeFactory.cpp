#include "BehaviorTreeFactory.h"

void SKNBT::BehaviorTreeFactory::RegisterCondition(std::string key, std::function<bool(void)> func)
{
	conditionTable.emplace(key, func);
}

void SKNBT::BehaviorTreeFactory::RegisterAction(std::string key, std::function<NodeStatus(void)>func)
{
	actionTable.emplace(key, func);
}

const std::function<SKNBT::NodeStatus(void)> SKNBT::BehaviorTreeFactory::GetActionFunc(const std::string& key)
{
	return actionTable.at(key);
}

const std::function<bool(void)> SKNBT::BehaviorTreeFactory::GetConditionFunc(const std::string& key)
{
	return conditionTable.at(key);
}

#pragma once
#include <unordered_map>
#include <string>
#include <functional>
#include "NodeBase.h"

class BehaviorTreeFactory
{
public:
	void RegisterCondition(std::string key, std::function<bool(void)> func);
	void RegisterAction(std::string key, std::function<NodeStatus(void)>func);

	const std::function<NodeStatus(void)> GetActionFunc(const std::string& key);
	const std::function<bool(void)> GetConditionFunc(const std::string& key);

private:
	std::unordered_map<std::string, std::function<bool(void)>> conditionTable;
	std::unordered_map<std::string, std::function<NodeStatus(void)>> actionTable;
};


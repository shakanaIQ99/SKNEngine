#pragma once
#include <unordered_map>
#include <string>
#include <functional>
#include "NodeBase.h"


class BehaviorTreeFactory
{
public:
	void RegisterCondition(std::string, std::function<bool()>);
	//void RegisterAction(std::string, std::function<SKNBT::NodeStatus>);

	/*const std::function<NodeStatus()> GetActionFunc(const std::string& key);
	const std::function<bool()> GetConditionFunc(const std::string& key);*/

private:
	std::unordered_map<std::string, std::function<bool()>> conditionTable;
	std::unordered_map<std::string, std::function<NodeStatus()>> actionTable;
};


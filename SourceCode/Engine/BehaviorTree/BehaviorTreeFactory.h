#pragma once
#include <unordered_map>
#include <string>
#include <functional>
#include "NodeBase.h"
namespace SKN {
	class NodeStatus;


	class BehaviorTreeFactory
	{
	public:
		void RegisterCondition(std::string, std::function<bool()>);
		void RegisterAction(std::string, std::function<NodeStatus(void)>);

		const std::function<SKN::NodeStatus(void)> GetActionFunc(const std::string& key);
		const std::function<bool()> GetConditionFunc(const std::string& key);

	private:
		std::unordered_map<std::string, std::function<bool()>> conditionTable;
		std::unordered_map<std::string, std::function<SKN::NodeStatus()>> actionTable;
	};

}
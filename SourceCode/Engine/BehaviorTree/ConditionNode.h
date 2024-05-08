#pragma once
#include "NodeBase.h"

    class ConditionNode :public NodeBase
    {
    public:
        void OnStart() override;
        NodeStatus Update() override;
        void OnEnd() override;
        void OnAbort() override;

        void SetParam(std::string param) override;

        void InitNode();
        std::string GetNodeType() override;

        int32_t selectedIndex = -1;
    };


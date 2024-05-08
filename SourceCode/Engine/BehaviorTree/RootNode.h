#pragma once
#include "NodeBase.h"



    class RootNode :public NodeBase
    {
    public:
        void OnStart() override;
        NodeStatus Update() override;
        void OnEnd() override;
        void OnAbort() override;

        void SetRootBT(BehaviorTree* bt);

        void SetParam(std::string Param) override;
        std::string GetNodeType() override;
    };


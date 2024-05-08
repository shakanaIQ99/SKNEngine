#pragma once
#include "NodeBase.h"

    class SequencerNode :public NodeBase
    {
    public:
        void OnStart() override;
        NodeStatus Update() override;
        void OnEnd() override;
        void OnAbort() override;

        void SetParam(std::string Param) override;

        void InitNode();

        std::string GetNodeType() override;
    };


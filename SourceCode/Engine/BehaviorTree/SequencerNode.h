#pragma once
#include "NodeBase.h"
namespace SKNBT
{
    class SequencerNode :public NodeBase
    {
    public:
        void OnStart() override;
        NodeStatus Update() override;
        void OnEnd() override;
        void OnAbort() override;

        void SetParam(std::string param) override;

        void InitNode();

        std::string GetNodeType() override;
    };

}
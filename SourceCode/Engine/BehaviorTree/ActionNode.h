#pragma once
#include"NodeBase.h"
namespace SKN {
    class ActionNode :public NodeBase
    {
    public:
        void OnStart() override;
        NodeStatus Update() override;
        void OnEnd() override;
        void OnAbort() override;

        void SetParam(std::string Param) override;

        void InitNode();

        std::string GetNodeType() override;

    private:
        std::string funcKey;
    };
}

#pragma once
#include"NodeBase.h"
namespace SKNBT
{
    class ActionNode :public NodeBase
    {
    public:
        void OnStart() override;
        NodeStatus Update() override;
        void OnEnd() override;
        void OnAbort() override;

        void SetParam(std::string param) override;

        void InitNode();

        std::string GetNodeType() override;

    private:
        std::string funcKey;
    };

}
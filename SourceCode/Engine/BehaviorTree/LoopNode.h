#pragma once
#include "NodeBase.h"
namespace SKN {
    class LoopNode :public NodeBase
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
        int32_t loopCount = 0;
        int32_t param = 0;
    };

}
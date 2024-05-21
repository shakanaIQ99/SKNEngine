#include "NodeBase.h"

void NodeBase::OnAbort()
{
	OnEnd();
}

void NodeBase::SetParam(std::string Param)
{
	param = Param;
}

std::string NodeBase::GetParam()
{
	return param;
}

NodeBase* NodeBase::GetParent()
{
	return parent;
}

const std::list<std::unique_ptr<NodeBase>>& NodeBase::GetChildren()
{
	return children;
}

NodeBase* NodeBase::Last()
{
	return children.back().get();
}

std::unique_ptr<NodeBase>* NodeBase::LastPtr()
{
	return &children.back();
}

std::unique_ptr<NodeBase>* NodeBase::ChangeParent(NodeBase* newParent)
{
	//両方ある場合
	if (parent != nullptr && newParent != nullptr)
	{
		for (auto itr = parent->children.begin(); itr != parent->children.end(); itr++)
		{
			if (itr->get() == this)
			{
				newParent->children.emplace_back();
				newParent->children.back().swap(*itr);
				itr = parent->children.erase(itr);
				this->parent = newParent;
				return &newParent->children.back();
			}
		}
	}
	//parentが無くてnew parentがある場合
	else if (parent == nullptr && newParent != nullptr)
	{
		if (parentBT->root.get() == this)
		{
			return &parentBT->root;
		}

		for (auto itr = parentBT->floatingNodes.begin(); itr != parentBT->floatingNodes.end(); itr++)
		{
			if (itr->get() == this)
			{
				newParent->children.emplace_back();
				newParent->children.back().swap(*itr);
				itr = parentBT->floatingNodes.erase(itr);
				this->parent = newParent;
				return &newParent->children.back();
			}
		}
	}
	//parentがあってnew parentが無い場合
	else if (parent != nullptr && newParent == nullptr)
	{
		for (auto itr = parent->children.begin(); itr != parent->children.end(); itr++)
		{
			if (itr->get() == this)
			{
				parentBT->floatingNodes.emplace_back();
				parentBT->floatingNodes.back().swap(*itr);
				itr = parent->children.erase(itr);
				this->parent = nullptr;
				return &parentBT->floatingNodes.back();
			}
		}
	}

	else //parentもnew parentもない場合
	{
		if (parentBT->root.get() == this)
		{
			return &parentBT->root;
		}

		for (auto itr = parentBT->floatingNodes.begin(); itr != parentBT->floatingNodes.end(); itr++)
		{
			if (itr->get() == this)
			{
				return &(*itr);
			}
		}
	}

	return nullptr;
}

#pragma once
#include"NodeBase.h"
#include"Vector2.h"
#include"GraphEditor.h"

class BehaviorTreeEditer;
class EditerNode
{
public:
	friend BehaviorTreeEditer;

	EditerNode(std::unique_ptr<SKN::NodeBase>* Node, std::string UniqueName, BehaviorTreeEditer* Master);

	void ChangeNodeType(std::string type);

	void Draw();

	const Vector2& GetPos();

	void SetComboBoxItem(std::string nodeType);
private:
	std::unique_ptr<SKN::NodeBase>* node;
	Vector2 pos;
	Vector2 size;

	GraphEditor::Node eNode;

	Vector2 firstPos = { 0.0f,0.0f };

	const char* currentItem = nullptr;

	std::string uniqueName;

	BehaviorTreeEditer* master;


};

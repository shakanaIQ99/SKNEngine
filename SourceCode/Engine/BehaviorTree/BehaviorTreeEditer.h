#pragma once
#include"EditerNode.h"
#include"BehaviorTree.h"


class BehaviorTreeEditer
{
public:

	static void Draw();

	//EditerNode* GetSelected();
	//void ClearSelected();
	//void SetSelected(EditerNode* node);

	//void DeleteNode(EditerNode* node);

	static void LoadFile(std::string filePath, GraphEditorDelegate Delegate);

	void Save(GraphEditor::Delegate &delegate);

	SKN::BehaviorTree tree;

	std::string filePath = "Resources/Boss.json";
public:
	BehaviorTreeEditer(const BehaviorTreeEditer&) = delete;
	BehaviorTreeEditer(BehaviorTreeEditer&&) = delete;
	BehaviorTreeEditer& operator=(const BehaviorTreeEditer&) = delete;
	BehaviorTreeEditer& operator=(BehaviorTreeEditer&&) = delete;

	static BehaviorTreeEditer* GetInstance();

private:
	BehaviorTreeEditer() = default;
	~BehaviorTreeEditer() = default;


};


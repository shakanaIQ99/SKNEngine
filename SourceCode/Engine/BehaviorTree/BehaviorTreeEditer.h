#pragma once
#include"EditerNode.h"
#include"BehaviorTree.h"
class BehaviorTreeEditer
{
public:

	static void Draw();

	EditerNode* GetSelected();
	void ClearSelected();
	void SetSelected(EditerNode* node);

	void DeleteNode(EditerNode* node);

	static void LoadFile(std::string filePath);


};


#pragma once
#include"PostEffect.h"
#include"AbstractSceneFactory.h"
#include"Framework.h"
class MyGame :public Framework
{
private:
	//シーンファクトリー
	std::unique_ptr<AbstractSceneFactory>sceneFactory;

	//ポストエフェクト
	std::unique_ptr<PostEffect>postEffect;

public:

	//初期化処理
	void Initialize()override;

	void LoadResource();

	//終了処理
	void Finalize()override;

	//更新処理
	void Update()override;

	//描画処理
	void SceneDraw()override;
	void PostEffectDraw()override;

};


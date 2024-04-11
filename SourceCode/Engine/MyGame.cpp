#include "MyGame.h"
#include "SceneManager.h"

void MyGame::Initialize()
{
	Framework::SetWindowData("Mecha Core");

	//基底クラスの初期化処理
	Framework::Initialize();

	//ポストエフェクトの初期化
	postEffect = std::make_unique<PostEffect>();
	postEffect->Initialize();

	//シーンマネージャーに最初のシーンをセット
	SceneManager::GetInstance()->ChangeScene("TITLE");

}

void MyGame::Finalize()
{
	SceneManager::GetInstance()->Finalize();

	//基底クラスの終了処理
	Framework::Finalize();
}

void MyGame::Update()
{
	//基底クラスの更新処理
	Framework::Update();

	//シーンマネージャーの更新処理
	SceneManager::GetInstance()->Update();

}

void MyGame::SceneDraw()
{
	postEffect->PreDrawScene();
	SceneManager::GetInstance()->Draw();
	postEffect->PostDrawScene();
}

void MyGame::PostEffectDraw()
{
	postEffect->Draw();
}

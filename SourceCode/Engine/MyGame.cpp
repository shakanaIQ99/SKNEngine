#include "MyGame.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include "TextureManager.h"

void MyGame::Initialize()
{
	Framework::SetWindowData("Mecha Core");

	//基底クラスの初期化処理
	Framework::Initialize();

	//ポストエフェクトの初期化
	postEffect = std::make_unique<PostEffect>();
	postEffect->Initialize();

	//シーンマネージャーに最初のシーンをセット
	SceneManager::GetInstance()->ChangeScene("GAME");

	LoadResource();

}

void MyGame::LoadResource()
{
	AudioManager::Load("Resources/Sound/wallhit.wav", "hit");//
	AudioManager::Load("Resources/Sound/shot.wav", "shot");//
	AudioManager::Load("Resources/Sound/enemyshot.wav", "Eshot");
	AudioManager::Load("Resources/Sound/mine.wav", "mine");//
	AudioManager::Load("Resources/Sound/charge.wav", "charge");//
	AudioManager::Load("Resources/Sound/jump.wav", "jump");//
	AudioManager::Load("Resources/Sound/dash.wav", "dash");//
	AudioManager::Load("Resources/Sound/homing.wav", "homing");
	AudioManager::Load("Resources/Sound/BGM.wav", "bgm");//



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

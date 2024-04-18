#include "MyGame.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include "TextureManager.h"
#include "SceneControl.h"

void MyGame::Initialize()
{
	Framework::SetWindowData("Mecha Core");

	//基底クラスの初期化処理
	Framework::Initialize();

	//ポストエフェクトの初期化
	postEffect = std::make_unique<PostEffect>();
	postEffect->Initialize();

	TextureManager::Load("Resources/Error.png", "ERRORTEXTURE");
	TextureManager::Load("Resources/white1x1.png", "EMPTYHANDOLE");
	LoadResource();




	//シーンマネージャーに最初のシーンをセット
	SceneManager::GetInstance()->ChangeScene("TITLE");
	SceneControl::GetInstance()->Initialize();




}

void MyGame::LoadResource()
{
	AudioManager::Load("Resources/Sound/wallhit.wav", "hit");
	AudioManager::Load("Resources/Sound/shot.wav", "shot");
	AudioManager::Load("Resources/Sound/enemyshot.wav", "Eshot");
	AudioManager::Load("Resources/Sound/mine.wav", "mine");
	AudioManager::Load("Resources/Sound/charge.wav", "charge");
	AudioManager::Load("Resources/Sound/jump.wav", "jump");
	AudioManager::Load("Resources/Sound/dash.wav", "dash");
	AudioManager::Load("Resources/Sound/homing.wav", "homing");
	AudioManager::Load("Resources/Sound/BGM.wav", "bgm");

	TextureManager::Load("Resources/title.png", "Title");
	TextureManager::Load("Resources/title2.png", "Title2");
	TextureManager::Load("Resources/scene.png", "Scene");
	TextureManager::Load("Resources/Clear.png", "Clear");
	TextureManager::Load("Resources/GameOver.png", "GameOver");
	TextureManager::Load("Resources/HpBar.png", "HpBar");
	TextureManager::Load("Resources/Reticle.png", "Reticle");
	TextureManager::Load("Resources/preKeyUI.png", "KeyUI");
	TextureManager::Load("Resources/Load.png", "Load");

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
	SceneControl::GetInstance()->Update();

}

void MyGame::SceneDraw()
{
	postEffect->PreDrawScene();
	SceneManager::GetInstance()->Draw();
	SceneControl::GetInstance()->Draw();
	postEffect->PostDrawScene();
}

void MyGame::PostEffectDraw()
{
	postEffect->Draw();
}

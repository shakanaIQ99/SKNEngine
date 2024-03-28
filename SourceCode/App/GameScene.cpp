#include "GameScene.h"
#include"FbxLoader.h"
#include"StuructTransform.h"
#include"Input.h"
#include"Collision.h"
#include"Easing.h"
#include"DeathParticle.h"
#include"AudioManager.h"

using namespace SKNEngine;

void GameScene::Init(DirectXCommon* dxcommon)
{
	spritecommon = new SpriteCommon();
	spritecommon->Initialize(dxcommon);

	texturemanager = TextureManager::GetInstance();
	texturemanager->StaticInitialize(dxcommon);
	light = LightGroup::Create();
	OBJ3D::SetLight(light);
	
	camera.Initialize(dxcommon->GetDevice());
	//Object3D::SetCamera(camera.getView());
	Draw3DLine::SetCamera(&camera);

	StuructTransform::SetStruct(&camera, spritecommon, texturemanager);

	//テクスチャ読み込み
	skydome_model = ObjModel::LoadFromOBJ("skydome",true);
	DeathParticle::SetModel(ObjModel::LoadFromOBJ("boxobj"));
	
	preTitleHandle = texturemanager->LoadTexture("Resources/title.png");
	preTitleHandle2 = texturemanager->LoadTexture("Resources/title2.png");
	SceneChaHandle = texturemanager->LoadTexture("Resources/scene.png");
	clearScHandle = texturemanager->LoadTexture("Resources/clear.png");
	GameOverScHandle = texturemanager->LoadTexture("Resources/GameOver.png");

	AudioManager::Load("Resources/Sound/aa.wav", "hit");
	AudioManager::Load("Resources/Sound/SE_spark.wav", "aaaa");
	

	//3Dモデル周り

	
	skydome = OBJ3D::Create();
	skydome->SetModel(skydome_model);

	field.Init(&camera);

	boss.Init();
	player.Init();

	camera.setTarget(&player.St->Wt.translation_);
	//camera.setTarget(&player.St->Wt);

	boss.SetPlayer(&player);
	player.SetEnemy(&boss.St->Wt);


	//スプライト周り

	preTitle = std::make_unique<Sprite2D>();
	preTitle->Initialize(spritecommon, preTitleHandle);
	preTitle->Wt.translation_ = { DxWindow::window_width / 2.0f,DxWindow::window_height / 4.5f ,0.0f };

	preTitle2 = std::make_unique<Sprite2D>();
	preTitle2->Initialize(spritecommon, preTitleHandle2);
	preTitle2->Wt.translation_ = { DxWindow::window_width / 2.0f,(DxWindow::window_height / 2.0f) + 60.0f ,0.0f };

	SceneCha = std::make_unique<Sprite2D>();
	SceneCha->Initialize(spritecommon, SceneChaHandle);
	SceneCha->Wt.translation_ = { DxWindow::window_width / 2.0f,(DxWindow::window_height / 2.0f) ,0.0f };

	clearSc = std::make_unique<Sprite2D>();
	clearSc->Initialize(spritecommon, clearScHandle);
	clearSc->Wt.translation_ = { DxWindow::window_width / 2.0f,(DxWindow::window_height / 2.0f) ,0.0f };


	//パーティクル周り




	light->SetDirLightActive(0, true);

	

	//light->SetDirLightColor(0, lightColor0);

	

	//float a = 0.2f;

	//field->Wt->translation_.y = -5.0f;
	player.Reset();
	boss.Reset();
	camera.Reset();
	BulletManager::clear();
}

void GameScene::Update()
{
	Vector3 lightDir0 = { 0,0,-1 };
	light->SetDirLightDir(0, lightDir0);
	
	switch (scene)
	{
	case SceneType::TITLE:
		TitleUpdate();
		if (Input::GetPadButtonDown(XINPUT_GAMEPAD_A) || Input::GetPressKey(DIK_END))
		{
			sceneChaflag = true;
		}

		if (sceneChaflag)
		{
			SceneChangeTimer++;
			if (SceneChangeTimer >= SceneChangeTime)
			{
				player.Reset();
				boss.Reset();
				BulletManager::clear();
				camera.Reset();
				GameUpdate();
				//camera.setPos(boss.GetPos());
				camera.setTarget(&player.prePlayer);
				scene = SceneType::GAMESCENE;
				
			}
		}
		else
		{
			SceneChangeTimer = 0;
		}
		break;
	case SceneType::GAMESCENE:
		if (sceneChaflag)
		{
			StartUpdate();
			
		}
		else
		{
			GameUpdate();
		}
		if (player.GameEnd() || boss.GameEnd())
		{
			endSceneChaflag = true;
			
		}
		if (endSceneChaflag)
		{
			SceneChangeTimer++;
			if (SceneChangeTimer >= SceneChangeTime)
			{
				if (player.GameEnd())
				{
					scene = SceneType::GAMEOVER;
					clearSc->SetTexture(GameOverScHandle);
				}
				else if(boss.GameEnd())
				{
					scene = SceneType::CLEARSCENE;
					clearSc->SetTexture(clearScHandle);
				}
				
				player.Reset();
				camera.Reset();
				endSceneChaflag = false;
			}
		}
		break;

	case SceneType::CLEARSCENE:
		
		SceneChangeTimer--;

		if (SceneChangeTimer <= 0)
		{
			SceneChangeTimer = 0;
			scene = SceneType::TITLE;
			player.Reset();
			camera.Reset();
		}

		break;

	case SceneType::GAMEOVER:

		SceneChangeTimer--;

		if (SceneChangeTimer <= 0)
		{
			SceneChangeTimer = 0;
			scene = SceneType::TITLE;
			player.Reset();
			camera.Reset();
		}


		break;
	}

	SceneAlpha = easeOutSine(0, 255.0f, static_cast<float>(SceneChangeTimer), static_cast<float>(SceneChangeTime));
	SceneCha->Wt.color = { SceneAlpha / 255.0f ,SceneAlpha / 255.0f ,SceneAlpha / 255.0f ,SceneAlpha / 255.0f };
	SceneCha->Update();
#ifdef _DEBUG
	ImGuiView();
#endif
	

}

void GameScene::Draw(DirectXCommon* dxcommon)
{
	switch (scene)
	{
	case SceneType::TITLE:
		TitleDraw(dxcommon);
		break;
	case SceneType::GAMESCENE:
		GameDraw(dxcommon);
		break;
	case SceneType::CLEARSCENE:
		spritecommon->PreDraw();
		clearSc->Draw();
		break;
	case SceneType::GAMEOVER:
		spritecommon->PreDraw();
		clearSc->Draw();
		break;
	}
	
	spritecommon->PreDraw();
	SceneCha->Draw();

	
	
}

void GameScene::ALLCol()
{
	const std::list<std::unique_ptr<Bullet>>& Bullets = BulletManager::GetBulletList();

	const std::list<std::unique_ptr<EnemyMine>>& enemyMines = boss.GetMines();

	Sphere playerSp;
	Sphere bossSp;

	playerSp.center = player.GetPos();
	playerSp.radius = player.St->Wt.scale_.x;

	bossSp.center = boss.St->Wt.translation_;
	bossSp.radius = boss.St->Wt.scale_.x;
	hitcheck = false;
	if (Collision::CheckSphereToSphere(playerSp, bossSp))
	{
		hitcheck = true;
		if (boss.GetAtkPattern() == AtkPattern::CHARGE)
		{
			player.Damege(0.2f);
			player.HitParticle(player.GetPos() - boss.GetPos());
			player.KnockBack(player.GetPos()-boss.GetPos());
		}
	}
	for (const std::unique_ptr<Bullet>& bullet : Bullets)
	{
		Sphere BulletSp;
		BulletSp.center = bullet->St->Wt.translation_;
		BulletSp.radius = bullet->St->Wt.scale_.x;
		if (Collision::CheckSphereToSphere(playerSp, BulletSp)&&bullet->tag==Tag::ENEMYNORMAL)
		{
			
			player.Damege(1.0f);
			player.HitParticle(bullet->GetVec());
			bullet->onDead();
		}
		if (Collision::CheckSphereToSphere(playerSp, BulletSp) && bullet->tag == Tag::ENEMYHARD)
		{

			player.Damege(3.0f);
			player.HitParticle(bullet->GetVec());
			bullet->onDead();
		}
		if (Collision::CheckSphereToSphere(bossSp, BulletSp) && bullet->tag == Tag::PLAYER)
		{

			boss.Damege(2.0f);
			boss.HitParticle(bullet->GetVec());
			bullet->onDead();
		}
		
	}
	for (const std::unique_ptr<EnemyMine>& mine : enemyMines)
	{
		Sphere bossMineSp;
		bossMineSp.center = mine->GetWorldPosition();
		bossMineSp.radius = mine->GetScale().x;
		if (Collision::CheckSphereToSphere(playerSp, bossMineSp)&&mine->IsHit())
		{

			player.Damege(5.0f);
			player.HitParticle(mine->GetVec());
			player.KnockBack(mine->GetVec());
			mine->OnCol();
			//mine->Destoroy();
		}

	}
	/*for (const std::unique_ptr<BulletManager>& p_bullet : playerBullets)
	{
		Sphere playerBulletSp;
		playerBulletSp.center =p_bullet->GetWorldPosition();
		playerBulletSp.radius = p_bullet->GetScale().x;
		if (Collision::CheckSphereToSphere(playerBulletSp, bossSp))
		{
			boss.Damege(2.0f);
			boss.HitParticle(p_bullet->GetVec());
			p_bullet->OnCollision();
		}
	}*/

}

void GameScene::TitleUpdate()
{
	if (tenmetu > 254.0f || tenmetu < 0.0f)
	{
		decri *= -1.0f;
	}

	tenmetu += decri;



	preTitle2->Wt.color = { tenmetu / 255.0f ,tenmetu / 255.0f ,tenmetu / 255.0f ,tenmetu / 255.0f };
	camera.Update();
	preTitle->Update();
	preTitle2->Update();
	player.TitleUpdate();
}

void GameScene::GameUpdate()
{


	ALLCol();

	light->Update();

	camera.Update();
	skydome->Update(camera.getView());
	field.Update();
	player.Update();
	boss.Update();
}

void GameScene::TitleDraw(DirectXCommon* dxcommon)
{
	OBJ3D::PreDraw(dxcommon->GetCommandList());
	/*skydome->Draw();
	field.Draw();*/
	player.Draw();
	spritecommon->PreDraw();

	preTitle->Draw();
	preTitle2->Draw();

	spritecommon->PostDraw();

}

void GameScene::GameDraw(DirectXCommon* dxcommon)
{
	OBJ3D::PreDraw(dxcommon->GetCommandList());

	skydome->Draw();
	field.Draw();

	BulletManager::ManageBulletUpdate();

	player.Draw();
	boss.Draw();
	//ParticleManager::PreDraw(dxcommon->GetCommandList());

	// 3Dオブクジェクトの描画
	//particleMan->Draw();


	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	//ParticleManager::PostDraw();

	spritecommon->PreDraw();
	if (!sceneChaflag)
	{
		player.DrawUI();
		boss.DrawUI();
	}

	//sprite->Draw({ 0,0 });
	//sprite2->DrawClip({ 80.0f,180.0f }, { 200.0f,100.0f }, {});

	spritecommon->PostDraw();

}

void GameScene::StartUpdate()
{
	SceneChangeTimer--;
	//SceneCameraTimer++;

	

	if (SceneChangeTimer <= 0)
	{
		SceneChangeTimer = 0;
		//sceneChaflag = false;
	}
	//sceneCamera=

	//camera.setPos();
	if (SceneCameraTimer >= SceneCameraTime)
	{
		SceneCameraTimer = SceneCameraTime;
		//camera.setTarget(&player.St->Wt);
		
	}
	//camera.Update();
	camera.Update();
	skydome->Update(camera.getView());
	field.Update();
	player.StartUpdate();
	boss.Update(sceneChaflag);
	

	if (player.Start())
	{
		sceneChaflag = false;
	}
}

void GameScene::ImGuiView()
{
	//ImguI
	ImGui::SetNextWindowPos({ ImGui::GetMainViewport()->WorkPos.x + 400, ImGui::GetMainViewport()->WorkPos.y + 10 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize({ 400, 500 });

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoResize;
	ImGui::Begin("Collsion", NULL, window_flags);

	ImGui::Checkbox("hit", &hitcheck);
	
	ImGui::End();
}



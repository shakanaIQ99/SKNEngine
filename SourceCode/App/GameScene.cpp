#include "GameScene.h"
#include"FbxLoader.h"
#include"StuructTransform.h"
#include"Input.h"
#include"Collision.h"
#include"Easing.h"
#include"DeathParticle.h"

void GameScene::Init(DirectXCommon* dxcommon)
{
	spritecommon = new SpriteCommon();
	spritecommon->Initialize(dxcommon);

	texturemanager = TextureManager::GetInstance();
	texturemanager->StaticInitialize(dxcommon);
	light = LightGroup::Create();
	OBJ3D::SetLight(light);
	camera.Initialize(dxcommon->GetDevice());
	Object3D::SetCamera(camera.getView());
	Draw3DLine::SetCamera(&camera);

	StuructTransform::SetStruct(&camera, spritecommon, texturemanager);

	//テクスチャ読み込み
	skydome_model = ObjModel::LoadFromOBJ("skydome");
	DeathParticle::SetModel(ObjModel::LoadFromOBJ("maru"));
	
	preTitleHandle = texturemanager->LoadTexture("Resources/title.png");
	preTitleHandle2 = texturemanager->LoadTexture("Resources/title2.png");
	SceneChaHandle = texturemanager->LoadTexture("Resources/scene.png");
	clearScHandle = texturemanager->LoadTexture("Resources/clear.png");
	GameOverScHandle = texturemanager->LoadTexture("Resources/GameOver.png");
	

	//3Dモデル周り

	
	skydome = OBJ3D::Create();
	skydome->SetModel(skydome_model);

	field.Init(&camera);

	boss.Init();
	player.Init();

	camera.setTarget(&player.St->Wt);

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

	float ambientColor[3] = { 1,1,1 };

	float lightDir0[3] = { 0,0,-1 };
	float lightColor0[3] = { 1,0,0 };

	float litX = 0.0f;
	float litY = 0.0f;
	float litZ = 0.0f;

	float pointLightPos[3] = { litX,litY,litZ };
	float pointLightColor[3] = { 1,1,1 };

	float pointLightAtten[3] = { 0.3f,0.1f,0.1f };

	light->SetDirLightActive(0, true);
	light->SetPointLightActive(0, false);


	light->SetPointLightPos(0, XMFLOAT3(pointLightPos));
	light->SetPointLightColor(0, XMFLOAT3(pointLightColor));
	light->SetPointLightAtten(0, XMFLOAT3(pointLightAtten));

	//float a = 0.2f;

	//field->Wt->translation_.y = -5.0f;
	player.Reset();
	boss.Reset();
	camera.Reset();
}

void GameScene::Update()
{
	
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
				camera.Reset();
				GameUpdate();
				//camera.setPos(boss.GetPos());
				camera.setTarget(&player.St->Wt);
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
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player.GetBullets();

	const std::list<std::unique_ptr<EnemyNormalBullet>>& enemyBullets = boss.GetBullets();

	Sphere playerSp;
	Sphere bossSp;

	playerSp.center = { player.GetPos().x, player.GetPos().y, player.GetPos().z ,1.0f };
	playerSp.radius = player.St->Wt.scale_.x;

	bossSp.center = { boss.St->Wt.translation_.x, boss.St->Wt.translation_.y,  boss.St->Wt.translation_.z ,1.0f };
	bossSp.radius = boss.St->Wt.scale_.x;

	if (Collision::CheckSphereToSphere(playerSp, bossSp))
	{
		if (boss.GetAtkPattern() == AtkPattern::CHARGE)
		{
			player.Damege(0.2f);
		}
	}
	for (const std::unique_ptr<EnemyNormalBullet>& bullet : enemyBullets)
	{
		Sphere bossBulletSp;
		bossBulletSp.center = { bullet->GetWorldPosition().x ,bullet->GetWorldPosition().y ,bullet->GetWorldPosition().z ,1.0f };
		bossBulletSp.radius = bullet->GetScale().x;
		if (Collision::CheckSphereToSphere(playerSp, bossBulletSp))
		{
			player.Damege(1.0f);
			bullet->OnCollision();
		}
		
	}
	for (const std::unique_ptr<PlayerBullet>& p_bullet : playerBullets)
	{
		Sphere playerBulletSp;
		playerBulletSp.center = { p_bullet->GetWorldPosition().x ,p_bullet->GetWorldPosition().y ,p_bullet->GetWorldPosition().z ,1.0f };
		playerBulletSp.radius = p_bullet->GetScale().x;
		if (Collision::CheckSphereToSphere(playerBulletSp, bossSp))
		{
			boss.Damege(2.0f);
			p_bullet->OnCollision();
		}
	}

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
	static XMVECTOR lightDir = { 0,1,5,0 };


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
}



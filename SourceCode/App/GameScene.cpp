#include "GameScene.h"
#include"FbxLoader.h"
#include"StuructTransform.h"
#include"Input.h"
#include"Collision.h"
#include"Easing.h"

void GameScene::Finalize()
{
	preTitle.reset();
	preTitle2.reset();

	spritecommon.reset();
	light.reset();
}

void GameScene::Init()
{
	spritecommon = std::make_unique<SpriteCommon>();
	spritecommon->Initialize();

	light= std::make_unique<LightGroup>();
	light.reset(LightGroup::Create());
	OBJ3D::SetLight(light.get());
	camera.Initialize();
	Object3D::SetCamera(camera.getView());
	Draw3DLine::SetCamera(&camera);

	StuructTransform::SetStruct(&camera, spritecommon.get());

	//テクスチャ読み込み
	
	preTitleHandle = TextureManager::GetInstance()->LoadTexture("Resources/title.png");
	preTitleHandle2 = TextureManager::GetInstance()->LoadTexture("Resources/title2.png");
	SceneChaHandle = TextureManager::GetInstance()->LoadTexture("Resources/scene.png");
	

	//3Dモデル周り
	//model.reset(FbxLoader::GetInstance()->LoadModelFlomFile("Robo"));
	
	//Robo = std::make_unique<Object3D>();
	//Robo->Initilaize();
	//Robo->SetModel(model.get());

	//Robo->wt.scale_ = { 1.0f,1.0f,1.0f};
	
	field.Init(&camera);

	boss.Init();
	player.Init();

	

	boss.SetPlayer(&player);
	player.SetEnemy(&boss.St->Wt);


	//スプライト周り

	preTitle = std::make_unique<Sprite2D>();
	preTitle->Initialize(spritecommon.get(), preTitleHandle);
	preTitle->Wt.translation_ = { DxWindow::window_width / 2.0f,DxWindow::window_height / 4.5f ,0.0f };

	preTitle2 = std::make_unique<Sprite2D>();
	preTitle2->Initialize(spritecommon.get(), preTitleHandle2);
	preTitle2->Wt.translation_ = { DxWindow::window_width / 2.0f,(DxWindow::window_height / 2.0f)+60.0f ,0.0f};

	SceneCha = std::make_unique<Sprite2D>();
	SceneCha->Initialize(spritecommon.get(), SceneChaHandle);
	SceneCha->Wt.translation_ = { DxWindow::window_width / 2.0f,(DxWindow::window_height / 2.0f) ,0.0f };


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

}

void GameScene::Update()
{
	
	switch (scene)
	{
	case SceneType::TITLE:
		TitleUpdate();
		if (Input::GetPadButtonDown(XINPUT_GAMEPAD_A)||Input::GetPressKey(DIK_END))
		{
			sceneChaflag = true;
		}

		if (sceneChaflag)
		{
			SceneChangeTimer++;
			if (SceneChangeTimer >= SceneChangeTime)
			{
				player.Reset();
				camera.setTarget(&player.St->Wt);
				boss.Reset();
				GameUpdate();
				scene = SceneType::GAMESCENE;
			}
		}
		break;
	case SceneType::GAMESCENE:
		if (sceneChaflag)
		{
			SceneChangeTimer--;
			if (SceneChangeTimer <=0)
			{
				sceneChaflag = false;
				SceneChangeTimer = 0;
			}
		}
		else
		{
			GameUpdate();
		}
		if (player.Death() || boss.Death())
		{
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

void GameScene::Draw()
{
	switch (scene)
	{
	case SceneType::TITLE:
		TitleDraw();
		break;
	case SceneType::GAMESCENE:
		GameDraw();
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
	field.Update();
	player.Update();
	boss.Update();
	//Robo->Update();
}

void GameScene::TitleDraw()
{
	field.Draw();
	player.Draw();
	spritecommon->PreDraw();

	preTitle->Draw();
	preTitle2->Draw();
}

void GameScene::GameDraw()
{

	field.Draw();
	//Robo->Draw();
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

	player.DrawUI();
	boss.DrawUI();

	//sprite->Draw({ 0,0 });
	//sprite2->DrawClip({ 80.0f,180.0f }, { 200.0f,100.0f }, {});


}

void GameScene::ImGuiView()
{

}



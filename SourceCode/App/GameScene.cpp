#include "GameScene.h"
#include"StuructTransform.h"
#include"Input.h"
#include"Collision.h"
#include"Easing.h"
#include"DeathParticle.h"
#include"AudioManager.h"
#include"TextureManager.h"
#include"SceneControl.h"

using namespace SKNEngine;

void GameScene::Initialize()
{
	
	light = LightGroup::Create();
	OBJ3D::SetLight(light);
	
	camera.Initialize();
	//Object3D::SetCamera(camera.getView());
	Draw3DLine::SetCamera(&camera);

	StuructTransform::SetStruct(&camera);

	skydome_model = ObjModel::LoadFromOBJ("skydome",true);
	DeathParticle::SetModel(ObjModel::LoadFromOBJ("boxobj"));
	

	//3Dモデル周り

	
	skydome = OBJ3D::Create();
	skydome->SetModel(skydome_model);

	field.Initialize(&camera);

	boss.Initialize();
	player.Initialize();

	camera.SetTarget(&player.St->Wt.translation_);
	//camera.setTarget(&player.St->Wt);

	boss.SetPlayer(&player);
	player.SetEnemy(&boss.St->Wt);


	//スプライト周り
	



	//パーティクル周り




	//light->SetDirLightColor(0, lightColor0);

	

	//float a = 0.2f;

	//field->Wt->translation_.y = -5.0f;
	player.Reset();
	boss.Reset();
	BulletManager::Clear();
	camera.Reset();
	GameUpdate();
	camera.SetTarget(&player.prePlayer);
	sceneChaflag = true;
	AudioManager::Play("bgm", 0.3f, true);
}

void GameScene::Update()
{

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
		SceneControl::GetInstance()->Change("RESULT");
		SceneControl::GetInstance()->SetWin(boss.GameEnd());
	}
		

#ifdef _DEBUG
	ImGuiMode();
#endif
	

}

void GameScene::Draw()
{
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

	SpriteCommon::PreDraw();
	if (!sceneChaflag)
	{
		player.DrawUI();
		boss.DrawUI();
	}

	//sprite->Draw({ 0,0 });
	//sprite2->DrawClip({ 80.0f,180.0f }, { 200.0f,100.0f }, {});

	
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
			AudioManager::Play("charge");

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

			player.Damege(2.0f);
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

void GameScene::GameUpdate()
{


	ALLCol();

	light->Update();

	camera.Update();
	skydome->Update(camera.GetView());
	field.Update();
	player.Update();
	boss.Update();
}


void GameScene::StartUpdate()
{
	sceneChangeTimer--;
	//SceneCameraTimer++;

	

	if (sceneChangeTimer <= 0)
	{
		sceneChangeTimer = 0;
		//sceneChaflag = false;
	}
	//sceneCamera=

	//camera.setPos();
	if (sceneCameraTimer >= sceneCameraTime)
	{
		sceneCameraTimer = sceneCameraTime;
		//camera.setTarget(&player.St->Wt);
		
	}
	//camera.Update();
	camera.Update();
	skydome->Update(camera.GetView());
	field.Update();
	player.StartUpdate();
	boss.Update(sceneChaflag);
	

	if (player.Start())
	{
		sceneChaflag = false;
	}
}

void GameScene::ImGuiMode()
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



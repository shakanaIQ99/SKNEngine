#include "GameScene.h"
#include"FbxLoader.h"
#include"StuructTransform.h"
#include"Input.h"
#include"Collision.h"

void GameScene::Init(DxWindow* dxwindow, DirectXCommon* dxcommon)
{
	spritecommon = new SpriteCommon();
	spritecommon->Initialize(dxcommon);

	texturemanager = TextureManager::GetInstance();
	texturemanager->StaticInitialize(dxcommon);
	uint32_t blank = texturemanager->LoadTexture("Resources/white1x1.png");
	light = LightGroup::Create();
	OBJ3D::SetLight(light);
	camera.Initialize(dxcommon->GetDevice());
	Object3D::SetCamera(camera.getView());
	Draw3DLine::SetCamera(&camera);

	StuructTransform::SetStruct(&camera, spritecommon, texturemanager);

	//テクスチャ読み込み
	skydome_model = ObjModel::LoadFromOBJ("skydome");
	field_model = ObjModel::LoadFromOBJ("ground");
	
	

	//3Dモデル周り

	
	skydome = OBJ3D::Create(&skydome_wt);
	skydome->SetModel(skydome_model);

	field = OBJ3D::Create(&field_wt);
	field->SetModel(field_model);

	boss.Init();
	player.Init();

	camera.setTarget(&player.transform);

	boss.SetPlayer(&player);
	player.SetEnemy(&boss.transform);

	field_wt.scale_ = {1.0f,5.0f,1.0f};


	//スプライト周り


	//パーティクル周り

	int scenenum = 0;

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
	static XMVECTOR lightDir = { 0,1,5,0 };

	/*cameraX = camera.GetWorldPosition().x;
	cameraZ = camera.GetWorldPosition().z;



	XMFLOAT2 inputnum = Input::GetRStick(true, true);
	cameraRotateY += (float)inputnum.x * cameraDPI;
	rotateY += (float)inputnum.x * cameraDPI;
	if ((cameraRotateX < 0.27f && (float)inputnum.y / SHRT_MAX>0) || (cameraRotateX > -0.6f && (float)inputnum.y / SHRT_MAX < 0))
	{
		cameraRotateX += (float)inputnum.y *cameraDPI ;
		rotateX -= (float)inputnum.y * cameraDPI;
	}*/

	ALLCol();

	light->Update();
	/*camera.setPos(XMFLOAT3((sinf(cameraRotateY) * 20 + player.GetPos().x), (sinf(-cameraRotateX) * 20 + player.GetPos().y + 5), (cosf(cameraRotateY) * 20 + player.GetPos().z)));
	camera.setRotate({ rotateX,rotateY,0 });*/
	camera.Update();
	skydome->Update(camera.getView());
	field->Update(camera.getView());
	player.Update();
	boss.Update();


}

void GameScene::Draw(DirectXCommon* dxcommon)
{


	OBJ3D::PreDraw(dxcommon->GetCommandList());

	skydome->Draw();
	field->Draw();

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

	spritecommon->PostDraw();

}

void GameScene::ALLCol()
{
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player.GetBullets();

	const std::list<std::unique_ptr<EnemyNormalBullet>>& enemyBullets = boss.GetBullets();

	Sphere playerSp;
	Sphere bossSp;

	playerSp.center = { player.GetPos().x, player.GetPos().y, player.GetPos().z ,1.0f };
	playerSp.radius = player.transform.scale_.x;

	bossSp.center = { boss.transform.translation_.x, boss.transform.translation_.y,  boss.transform.translation_.z ,1.0f };
	bossSp.radius = boss.transform.scale_.x;

	if (Collision::CheckSphereToSphere(playerSp, bossSp))
	{

	}
	for (const std::unique_ptr<EnemyNormalBullet>& bullet : enemyBullets)
	{
		Sphere bossBulletSp;
		bossBulletSp.center = { bullet->GetWorldPosition().x ,bullet->GetWorldPosition().y ,bullet->GetWorldPosition().z ,1.0f };
		bossBulletSp.radius = bullet->GetScale().x;
		if (Collision::CheckSphereToSphere(playerSp, bossBulletSp))
		{
			player.Damege(1.0f);
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
		}
	}

}



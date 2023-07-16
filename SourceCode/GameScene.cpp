#include "GameScene.h"
#include"FbxLoader.h"


void GameScene::Init(DxWindow* dxwindow, DirectXCommon* dxcommon)
{
	
	imGuiManager = new ImGuiManager();
	imGuiManager->Initialize(dxwindow->GetHwnd(), dxcommon);

	input = new Input();
	input->Initialize(dxwindow->GetHInstance(), dxwindow->GetHwnd());

	spritecommon = new SpriteCommon();
	spritecommon->Initialize(dxcommon);

	texturemanager = TextureManager::GetInstance();
	texturemanager->StaticInitialize(dxcommon);
	uint32_t blank = texturemanager->LoadTexture("Resources/white1x1.png");
	light = LightGroup::Create();
	OBJ3D::SetLight(light);
	camera.Initialize({}, {},dxcommon->GetDevice());
	Object3D::SetCamera(camera.getView());

	//テクスチャ読み込み

	tex1 = texturemanager->LoadTexture("Resources/visual.png");
	tex2 = texturemanager->LoadTexture("Resources/puragomi.jpg");
	tex3 = texturemanager->LoadTexture("Resources/effect1.png");
	tex4 = texturemanager->LoadTexture("Resources/effect3.png");
	
	model1 = FbxLoader::GetInstance()->LoadModelFlomFile("boneTest");
	skydome_model = ObjModel::LoadFromOBJ("skydome");
	field_model = ObjModel::LoadFromOBJ("ground");
	p_model = ObjModel::LoadFromOBJ("player");
	

	//3Dモデル周り

	atm.CreateConstBuffer(dxcommon->GetDevice());

	objec1 = new Object3D();
	objec1->Initilaize(&atm);
	objec1->SetModel(model1);

	skydome = OBJ3D::Create(&skydome_wt);
	skydome->SetModel(skydome_model);

	field = OBJ3D::Create(&field_wt);
	field->SetModel(field_model);

	player.SetStruct(p_model, OBJ3D::Create(&player_wt), &camera,input);
	player.Init();


	//スプライト周り

	sprite = new Sprite2D();
	sprite->Initialize(spritecommon, &wt3, tex1);
	sprite2 = new Sprite2D();
	sprite2->Initialize(spritecommon, &wt4, tex2);

	//パーティクル周り

	particleMan = ParticleManager::Create(tex3, &p1);

	for (int i = 0; i < 100; i++)
	{
		const float rnd_pos = 20.0f;
		XMFLOAT3 pos{};
		pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		particleMan->Add(150, pos, vel, acc, 1.0f, 0.0f);

	}


	 

	eye = { 0,20,-200 };
	int scenenum = 0;

	float ambientColor[3] = { 1,1,1 };

	float lightDir0[3] = { 0,0,-1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

	float pointLightPos[3] = { 0.5f,1.0f,0 };
	float pointLightColor[3] = { 1,1,1 };

	float pointLightAtten[3] = { 0.3f,0.1f,0.1f };

	light->SetDirLightActive(0, true);
	light->SetDirLightActive(1, true);
	light->SetDirLightActive(2, true);
	light->SetPointLightActive(0, false);


	light->SetPointLightPos(0, XMFLOAT3(pointLightPos));
	light->SetPointLightColor(0, XMFLOAT3(pointLightColor));
	light->SetPointLightAtten(0, XMFLOAT3(pointLightAtten));

	//float a = 0.2f;

	atm.rotation_.y = XMConvertToRadians(90.0f);

	sprite2->Wt->translation_.y = 5.0f;
	wt3.translation_ = { 100.0f,100.0f,0.0f };
	wt3.scale_ = { 0.5f,0.5f,0 };
	wt4.scale_ = { 0.5f,0.5f,0 };

	

	//objec1->PlayAnimation();

	//field->Wt->translation_.y = -5.0f;

}

void GameScene::Update()
{

	input->InputUpdate();
	imGuiManager->Begin();
	static XMVECTOR lightDir = { 0,1,5,0 };

	cameraX = camera.GetWorldPosition().x;
	cameraZ = camera.GetWorldPosition().z;

	XMFLOAT2 inputnum = input->GetRStick(true, true);
	cameraRotateY += (float)inputnum.x / SHRT_MAX * 0.02f;
	rotateY += (float)inputnum.x / SHRT_MAX * 0.02f;
	if ((cameraRotateX < 0.27f && (float)inputnum.y / SHRT_MAX>0) || (cameraRotateX > -0.6f && (float)inputnum.y / SHRT_MAX < 0))
	{
		cameraRotateX += (float)inputnum.y / SHRT_MAX * 0.02f;
		rotateX -= (float)inputnum.y / SHRT_MAX * 0.02f;
	}

	particleMan->Update(camera.getView());

	light->Update();


	sprite->Update();
	sprite2->Update();

	XMFLOAT3 cameraPos = XMFLOAT3(player.GetPos().x - (flontVec.x * cameraDistance),
		player.GetPos().y - (flontVec.y * cameraDistance)+5.0f,
		player.GetPos().z - (flontVec.z * cameraDistance));

	XMFLOAT3 cameraVec = XMFLOAT3((cameraPos.x + (flontVec.x * cameraDistance)), (cameraPos.y + (flontVec.y * cameraDistance)), (cameraPos.z + (flontVec.z * cameraDistance)));

	camera.setPos(XMFLOAT3((sinf(cameraRotateY) * 20 + player.GetPos().x), (sinf(-cameraRotateX) * 20 + player.GetPos().y + 5), (cosf(cameraRotateY) * 20 + player.GetPos().z)));
	camera.setRotate({ rotateX,rotateY,0 });
	camera.Update();
	objec1->Update();
	skydome->Update(camera.getView());
	field->Update(camera.getView());
	player.Update();



	imGuiManager->End();
}

void GameScene::Draw(DirectXCommon* dxcommon)
{
	objec1->Draw(dxcommon->GetCommandList());
	OBJ3D::PreDraw(dxcommon->GetCommandList());

	skydome->Draw();
	field->Draw();

	player.Draw();
	OBJ3D::PostDraw();
	ParticleManager::PreDraw(dxcommon->GetCommandList());

	// 3Dオブクジェクトの描画
	//particleMan->Draw();


	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	ParticleManager::PostDraw();

	spritecommon->PreDraw();

	//sprite->Draw({ 0,0 });
	//sprite2->DrawClip({ 80.0f,180.0f }, { 200.0f,100.0f }, {});

	spritecommon->PostDraw();

	imGuiManager->Draw();
}



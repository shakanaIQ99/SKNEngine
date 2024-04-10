#pragma once
#include"DxWindow.h"
#include"DirectXCommon.h"
#include"OBJ3D.h"
#include"Input.h"
#include<wrl.h>
#include"LightGroup.h"
#include<sstream>
#include<iomanip>
#include"ImGuiManager.h"
#include"FPS.h"
#include"ParticleManager.h"
#include"BulletManager.h"
#include"PostEffect.h"
#include"SpriteCommon.h"
#include"Draw3DLine.h"
#include"AudioManager.h"
#include"myMath.h"

class Framework
{
protected:

	std::wstring title = L"";
	float windowWidth = 0.0f;//横幅
	float windowHeight = 0.0f;//縦幅

	Float4 windowColor = { 0.0f ,0.0f ,0.0f ,0.0f };

	bool endRequest = false;

	std::unique_ptr<DxWindow> dxWindow;
	std::unique_ptr<FPS>fps;

public:

	virtual ~Framework() = default;

	//初期化処理
	virtual void Initialize();

	//終了処理
	virtual void Finalize();

	//更新処理
	virtual void Update();

	//描画処理
	virtual void SceneDraw() = 0;
	virtual void PostEffectDraw() = 0;

	//終了フラグの取得
	virtual bool& GetEndRequest();


public:

	/// <summary>
	/// ウィンドウの設定
	/// </summary>
	/// <param name="title">タイトルバーの名前</param>
	/// <param name="width">ウィンドウの横幅</param>
	/// <param name="height">ウィンドウの縦幅</param>
	void SetWindowData(const std::string& Title = "DirectXGame", const float Width = 1280.0f, const float Height = 720.0f);

	/// <summary>
	/// ウィンドウの色を変更
	/// </summary>
	/// <param name="color"></param>
	void SetWindowColor(const Float4& color = { 0.1f,0.25f,0.5f,0.0f });

	//実行処理
	void Run();
};


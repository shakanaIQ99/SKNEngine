//#include "EnemyNormalBullet.h"
//
//std::unique_ptr<ObjModel> EnemyNormalBullet::Premodel;
//
//EnemyNormalBullet::EnemyNormalBullet()
//{
//	ModelInit(Premodel.get());
//	Velocity_ = { 0,0,0 };
//}
//
//EnemyNormalBullet::~EnemyNormalBullet()
//{
//}
//
//void EnemyNormalBullet::SetModel(ObjModel* _model)
//{
//	Premodel.reset(_model);
//}
//
//void EnemyNormalBullet::Initlize(const Vector3& position, const Vector3& rota, const Vector3& velocity, const Tag& _tag)
//{
//	St->Wt.translation_ = position;
//	St->Wt.rotation_ = rota;
//	St->Wt.scale_ = { 0.5f,0.5f,0.5f };
//	St->color = { 1.0f,0,0,1.0f };
//
//	Velocity_ = velocity;
//
//	SetTag(_tag);
//}
//
//void EnemyNormalBullet::Update()
//{
//	St->Wt.translation_ = St->Wt.translation_ + Velocity_;
//
//	St->Update(camera->getView());
//
//	bulletLifeUpdate();
//
//}
//
//void EnemyNormalBullet::Draw()
//{
//	St->Draw();
//}

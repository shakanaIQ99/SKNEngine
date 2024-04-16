#pragma once
#include"Vector3.h"
class DirectionLight
{

public:

	struct ConstBufferData
	{
		bool active;
		Vector3 lightv;
		Vector3 lightcolor;
		float pad2;
	};



	inline void SetLightDir(const Vector3& _lightdir) { lightdir = _lightdir.GetNormalize(); }
	inline void SetLightColor(const Vector3& _lightcolor){lightcolor = _lightcolor;}

	inline Vector3 GetLightDir() { return lightdir; }
	inline Vector3 GetLightColor() { return lightcolor; }
	inline void SetActive(bool Active) { active = Active; }
	inline bool IsActive() { return active; }

private:


	Vector3 lightdir = { 1.0f,0,0};
	Vector3 lightcolor = { 1.0f,1.0f,1.0f };
	bool active = false;

};


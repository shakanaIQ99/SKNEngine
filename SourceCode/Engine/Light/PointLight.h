#pragma once
#include"Vector3.h"

class PointLight
{

public:

	struct ConstBufferData
	{
		Vector3 lightpos;
		float pad1;
		Vector3 lightcolor;
		float pad2;
		Vector3 lightatten;
		unsigned int active;
	};

	inline void SetLightPos(const Vector3& _lightpos) { lightpos = _lightpos; }
	inline const Vector3& GetLightPos() { return lightpos; }
	inline void SetLightColor(const Vector3& _lightcolor) { lightcolor = _lightcolor; }
	inline const Vector3& GetLightColor() { return lightcolor; }
	inline void SetLightAtten(const Vector3& _lightatten) { lightatten = _lightatten; }
	inline const Vector3& GetLightAtten() { return lightatten; }
	inline void SetActive(bool Active) { this->active = Active; }
	inline bool IsActive() { return active; }

private:

	Vector3 lightpos = { 0,0,0 };
	Vector3 lightcolor = { 1,1,1 };
	Vector3 lightatten = { 1.0f,1.0f,1.0f };

	bool active = false;

};


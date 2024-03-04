#pragma once
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include<string>
#include"myMath.h"
using namespace std;


struct PNU
{
	Vector3 pos; // xyz座標
	Vector3 normal;
	Vector2 uv;
};

struct ConstBufferDataMaterial
{
	Vector3 ambient;
	float pad1;
	Vector3 diffuse;
	float pad2;
	Vector3 specular;
	float alpha;
};

struct Material
{
	string name;
	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;

	float alpha;
	string textureFilename;

	Material()
	{
		ambient = { 0.3f,0.3f,0.3f };
		diffuse = { 0.0f,0.0f,0.0f };
		specular = { 0.0f,0.0f,0.0f };
		alpha = 1.0f;

	}
	ConstBufferDataMaterial GetConstBufferMaterial();
};





class DrawCommon
{
};


#pragma once
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include<string>
#include <DirectXMath.h>
using namespace std;

using namespace DirectX;

struct PNU
{
	XMFLOAT3 pos; // xyzç¿ïW
	XMFLOAT3 normal;
	XMFLOAT2 uv;
};

struct ConstBufferDataMaterial
{
	XMFLOAT3 ambient;
	float pad1;
	XMFLOAT3 diffuse;
	float pad2;
	XMFLOAT3 specular;
	float alpha;
};

struct Material
{
	string name;
	XMFLOAT3 ambient;
	XMFLOAT3 diffuse;
	XMFLOAT3 specular;

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


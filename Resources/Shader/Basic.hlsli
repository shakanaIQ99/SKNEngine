cbuffer ConstBufferDataMaterial:register(b0)
{
    matrix viewproj;
	matrix mat;
    float3 cameraPos;
	float4 color;
};

//cbuffer ConstBufferDataMaterial:register(b1)
//{
//	matrix mat;
//};

struct VSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv :TEXCOORD;
};
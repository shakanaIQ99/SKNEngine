cbuffer ConstBufferDataMaterial:register(b0)
{
	float4 color;
    matrix viewproj;
	matrix mat;
    float3 cameraPos;
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
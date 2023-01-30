cbuffer ConstBufferDataMaterial:register(b0)
{
	float4 color;
	matrix viewproj;
    matrix world;
    float3 cameraPos;
};

cbuffer ConstBufferDataMaterial1:register(b1)
{
	float3 m_ambient : packoffset(c0);
	float3 m_diffuse : packoffset(c1);
	float3 m_specular : packoffset(c2);
	float m_alpha : packoffset(c2.w);
};

cbuffer cbuff2 : register(b2)
{
    float3 lightv;
    float3 lightcolor;
}

struct VSOutput
{
	float4 svpos : SV_POSITION;
	//float3 normal :NORMAL;
	float4 color : COLOR;
	float2 uv :TEXCOORD;
};
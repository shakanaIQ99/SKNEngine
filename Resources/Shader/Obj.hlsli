cbuffer ConstBufferDataMaterial:register(b0)
{
	matrix viewproj;
    matrix world;
    float3 cameraPos;
	float4 color;
};

cbuffer ConstBufferDataMaterial1:register(b1)
{
	float3 m_ambient : packoffset(c0);
	float3 m_diffuse : packoffset(c1);
	float3 m_specular : packoffset(c2);
	float m_alpha : packoffset(c2.w);
};


struct DirLight
{
    uint active;
    float3 lightv;
    float3 lightcolor;
};

struct PointLight
{
    float3 lightpos;
    float3 lightcolor;
    float3 lightatten;
    uint active;
};

cbuffer cbuff2 : register(b2)
{
    float3 ambientColor;
    DirLight dirLights;
    PointLight pointLights;
}

struct VSOutput
{
	float4 svpos : SV_POSITION;
	float4 worldpos : POSITION;
    float3 normal : NORMAL;
	float2 uv :TEXCOORD;
};